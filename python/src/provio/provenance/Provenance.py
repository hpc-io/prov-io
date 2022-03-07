import RDF
import time
import uuid


DEFAULT_VERSION = 1.0
BASE_URI = 'http://www.w3.org/ns/provio#'
PREFIX = 'provio:'
PROV_FORMAT = ['turtle','xml']

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
				self._model = RDF.Model(self.storage)
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


		# Need to implement periodically serialization
		if 'Periodical' in args:
			if args['Serializer'] == True:
				pass

		# if 'Enable_ID' in args:

		# if 'Enable_ID' in args:


		if 'Enable_ID' in args:
			if args['Enable_ID'] == False:
				self._enable_id = False

		if _enable_id == True:
			self._id = uuid.uuid4().__str__()

		if 'Subclass' in args:
			# if args['Subclass'] in 
			pass

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

	def set_identifier:
		pass

	def add_subclass(new_class):
		return 0



def main():
	extensible = Extensible(version = 1.1)
	extensible._display_fields()
	# extensible = Extensible()


if __name__ == '__main__':
	main()