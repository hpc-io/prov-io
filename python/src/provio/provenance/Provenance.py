import RDF
import uuid
from abc import abstractmethod
from provio.extensible import Extensible


DEFAULT_VERSION = 1.0
BASE_URI = 'http://provio#'
PREFIX = 'provio'
DEFAULT_FORMAT = 'turtle'
PROV_FORMAT = ['turtle','xml','ntriples']

class Provenance():
	"""
	A wrapper class on top of Redland librdf
	"""
	_id = None
	_version = None
	_base_uri = None
	_prefix = None
	_enable_id = False
	_storage = None
	_model = None
	_serializer = None
	# _subclasses = None
	_parser = None
	_subgraph = None
	_period = None
	_op_count = None
	_first_write = None
	_enable_version = None
	_format = None
	_serialize_path = None

	def __init__(self, arg = None, **args):
		# A unique ID of a provenance instance
		self._enable_version = True
		self._version = 'v'+str(DEFAULT_VERSION)
		self._base_uri = BASE_URI
		self._prefix = PREFIX
		# self._subclasses = Extensible('')
		self._first_write = True
		self._format = DEFAULT_FORMAT
		self._op_count = 0

		if 'enable_version' in args:
			self._enable_version = args['enable_version']
		
		if 'enable_id' in args:
			if args['enable_id'] == True:
				self._enable_id = True
				self._id = uuid.uuid4().__str__()

		if 'storage' in args:
			'''
			Support BDB and Virtuoso
			'''
			if args['storage'] == bdb:
				self._storage = RDF.Storage(storage_name="hashes",
				                    name="test",
				                    options_string="new='yes',hash-type='memory',dir='.'")

			elif args['storage'] == virtuoso:
				pass

			else: 
				print('Unsupported version type')

			if self._storage is None:
				raise Exception("new RDF.Storage failed")

			else:
				self._model = RDF.Model(self._storage)
				if model is None:
					raise Exception("new RDF.model with storage failed")
		else:
			self._model = RDF.Model()

		if self._enable_version:		
			if 'set_version' in args:
				try: 
					_version = str(args['set_version'])
				except:
					print('Unsupported version type')
				self._version = 'v'+_version


		if 'load_provenance' in args:
			'''
			Load provenance from a file
			'''
			if 'load_format' not in args:
				pass
			else:
				self._format = args['load_format']
			print('Parsing as format: ' + self._format)
			self._parser = RDF.Parser(self._format)
			self._parser.parse_into_model(self._model, 'file:' + args['load_provenance'], ' ')

		if 'serializer' in args:
			'''
			Support turtle, xml, ntriples
			'''
			if args['serializer'] in PROV_FORMAT:
				self._format = args['serializer']
				self._serializer = RDF.Serializer(name = self._format)
				self._serializer.set_namespace(self._prefix, RDF.Uri(self._base_uri))
			else:
				raise Exception('unrecognized serialization format') 

		if 'periodical' in args:
			'''
			Periodically dump the in memory provio instance to disk every n 
			Need to initialize the serializer first
			Conflicts with enable_id
			'''
			if self._enable_id == False:
				self._period = args['periodical']
				self._parser = RDF.Parser(self._format)
				print('periodically serialize the graph every %d graph insertion' % self._period)
			else:
				print('To periodically serialize the graph, set enable_id to False')
				

	@abstractmethod
	def add_triple(self, triple):
		raise NotImplementedError

	@abstractmethod
	def set_prefix_and_base_uri(self, prefix, uri):
		raise NotImplementedError

	@abstractmethod
	def serialize_prov_to_file(self, path):
		raise NotImplementedError

	@abstractmethod
	def set_version(self, arg = None):
		raise NotImplementedError

	@abstractmethod
	def set_identifier(self):
		raise NotImplementedError

	# @abstractmethod
	# def display_all_subclasses(self):
	# 	raise NotImplementedError
