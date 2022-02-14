import RDF
import time
import uuid
import json
import os


'''
JSON file path of predefined subclasses
including: 
Configuration
Metrics
(Workflow) Types
'''
DEFAULT_SUBCLASS_PATH = '../../'

DEFAULT_VERSION = 1.0
DEFAULT_SUBCLASS = 'DefaultSubclass'
DEFAULT_FIELD = 'DefaultField'
BASE_URI = 'http://www.w3.org/ns/provio#'
PREFIX = 'provio:'
PROV_FORMAT = ['turtle','xml']


class Extensible():
	"""
	PROV-IO Extensible Class
	"""
	def __init__(self, arg = None, **args):

		self._subclass = DEFAULT_SUBCLASS
		self._field = DEFAULT_FIELD
		self._subclass_path = DEFAULT_SUBCLASS_PATH
		self._subclass_dict = {}

		if 'SubclassPath' in args:
			self._subclass_path = args['SubclassPath']

		for subclass_file in os.listdir(self._subclass_path):
			subclass_name = os.fsdecode(subclass_file)
			if subclass_name.endswith(".json"):
				with open(subclass_name) as subclass_json:
					subclass = json.load(subclass_json)
					self._subclass_dict[subclass_name.split('.')[0]] = subclass

		for files in os.listdir(directory)


		if arg:
			self._field = arg

		if 'Subclass' in args:
			self._subclass = args['Subclass']


	def _set_field():
		if field not in fields:
			this.field = field

	def _display_fields(arg):
		for field in this.arg:
			print(field)

class Provenance():

	def __init__(self, arg = None, **args):
		self._version = DEFAULT_VERSION

		self._base_uri = BASE_URI
		self._prefix = PREFIX
		self._enable_id = True
		self._storage = None
		self._model = None
		self._serializer = None

		if 'Storage' in args:
			"""
			Currently only support BDB storage
			"""
			self._storage = RDF.Storage(storage_name="hashes",
			                    name="test",
			                    options_string="new='yes',hash-type='memory',dir='.'")

			if self.storage is None:
				raise Exception("new RDF.Storage failed")

			else:
				self._model = RDF.Model(self.storage):
				if model is None:
					raise Exception("new RDF.model with storage failed")
		else:
			self._model = RDF.Model("new RDF.model without storage failed")

		serializer=RDF.Serializer()
		if 'Serializer' in args:
			"""
			Support turtle, xml, etc
			"""
			if args['Serializer'] in PROV_FORMAT:
				_format = args['Serializer']
				
				serializer.set_namespace("dc", RDF.Uri("http://purl.org/dc/elements/1.1/"))
			else:
				raise Exception('unrecognized serialization format') 

		if arg:
			self._field = arg
		else:
			raise Exception('please provide the subject of triple')


		



		if 'Enable_ID' in args:

		if 'Enable_ID' in args:


		if 'Enable_ID' in args:
			if args['Enable_ID'] == False:
				self._enable_id = False

		if _enable_id == True:
			self._id = uuid.uuid4().__str__()

		if 'Subclass' in args:
			if args['Subclass'] in 

	def new_triple():
		return RDF.Statement(RDF.Uri("http://www.dajobe.org/"),
                        RDF.Uri("http://purl.org/dc/elements/1.1/creator"),
                        RDF.Node("Dave Beckett"))

	def add_triple_to_graph(triple):
		try:
			self.model.add_statement(statement)
		except:
			raise Exception('failed to add new triple to provenance graph')


	def set_prefix_and_base_uri(prefix, uri):
		if (self._serializer):
			try:
				serializer.set_namespace(prefix, RDF.Uri(uri))
			except:
				raise Exception('failed to set prefix and base uri') 


	def serialize_prov_to_file(path):
		if (self._serializer):
			try:
				_serializer.serialize_model_to_file(path, self.model)
			except:
				raise Exception('failed to serialize provenance graph to %s' % path) 


	def _set_version(self, arg = None):
		if arg:
			self._version = RDF.Node(args['version'])
		else:
			print('WARNING: version information not provided.')
			return 1

	def add_subclass(new_class):
		return 0








def main():
	extensible = Extensible(version = 1.1)
	extensible._display_fields()
	# extensible = Extensible()


if __name__ == '__main__':
	main()