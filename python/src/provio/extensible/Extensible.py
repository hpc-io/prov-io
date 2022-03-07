import json
import os
from abc import abstractmethod


"""
JSON file path of predefined subclasses
including: 
Configuration
Metrics
(Workflow) Types
"""
DEFAULT_SUBCLASS_PATH = '../extensible_class/' # Replace with your path
DEFAULT_SUBCLASS = 'DefaultSubclass'
DEFAULT_FIELD = 'DefaultField'


def merge_two_dicts(x, y):
	"""
	Helper function to merge dictionaries into one
	"""
	z = x.copy()   
	z.update(y)  
	return z


class subclass():
	"""
	PROV-IO Extensible Class
	"""
	def __init__(self, arg = None, **args):
		"""
		Constructor
		arg subclass field
		args SubclassPath: subclass json file path
		"""
		self._subclass = DEFAULT_SUBCLASS
		self._field = DEFAULT_FIELD
		self._subclass_path = DEFAULT_SUBCLASS_PATH
		self._subclass_dict = {}

		if 'SubclassPath' in args:
			self._subclass_path = args['SubclassPath']

		'''
		Load provenance subclasses from given path
		Allow multiple subclass json files in the path
		'''
		for subclass_file in os.listdir(self._subclass_path):
			subclass_name = os.fsdecode(subclass_file)
			if subclass_name.endswith(".json"):
				with open(self._subclass_path + subclass_name) as subclass_json:
					try:
						_dict = json.load(subclass_json)
					except:
						print('Failed to load subclass file "%s"' % (self._subclass_path + subclass_name))
					self._subclass_dict =  merge_two_dicts(self._subclass_dict, _dict)
		'''
		Look for the given field in dictionary
		'''
		if arg:
			self._field = arg
			try:
				self._subclass = self._subclass_dict[arg]
			except Exception:
				pass
		else:
			print('warning: blank provenance node')

	@abstractmethod
	def display_subclass_fields(self, subclass):
		raise NotImplementedError

	@abstractmethod
	def display_all_subclasses(self):
		raise NotImplementedError

	@abstractmethod
	def add_subclass(self, subclass, fields):
		raise NotImplementedError
		