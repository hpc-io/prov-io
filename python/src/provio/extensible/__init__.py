import json
import os
from provio.extensible.Extensible import subclass


class Extensible(subclass):

	def display_subclass_fields(self, subclass):
		"""
		Display fields of a subclass
		"""
		fields = ''
		count = 0
		for field in self._subclass_dict:
			if self._subclass_dict[field] == subclass:
				fields += field + ', '
				count += 1
		print('Found %d fields in subclass %s: %s' % (count, subclass, fields[:-2]))

	def display_all_subclasses(self):
		"""
		Display all subclasses and their fields
		"""
		subclass_dict_reverse = {}
		for field in self._subclass_dict:
			try: 
				subclass_dict_reverse[self._subclass_dict[field]].append(field)
			except:
				subclass_dict_reverse[self._subclass_dict[field]] = [field]

		for subclass in subclass_dict_reverse:
			print('"%s": %s' % (subclass, ', '.join(subclass_dict_reverse[subclass])))

	def add_subclass(self, subclass, fields):
		"""
		For users to add their own subclasses or add new fields to their own subclasses
		Provide subclass as a string and fields could be a string or a list of string
		Serialize newly added subclasses to existing file or a new file
		"""
		subclass_new = {}
		try: 
			subclass_json = open(self._subclass_path + subclass + '.json')
			subclass_new = json.load(subclass_json)
		except:
			print('New subclass: %s'% subclass)
			
		with open(self._subclass_path + subclass + '.json', 'w') as subclass_json:

			if isinstance(fields, list):
				for field in fields:
					if field not in self._subclass_dict:
						self._subclass_dict[field] = subclass
						subclass_new[field] = subclass
						print(subclass_new)
					else:
						print('Field %s already exists' % field)
				print('Added subclass "%s" with fields: ' % subclass + ', '.join(fields))

			elif isinstance(fields, str):
				if fields not in self._subclass_dict:
						self._subclass_dict[fields] = subclass
						subclass_new[fields] = subclass
						print(subclass_new)
				print('Added subclass %s: ' % subclass + fields)

			else:
				print('Parameter "fields" must be a string or a list')

			try:
				print(subclass_new)
				json.dump(subclass_new, subclass_json)

			except Exception:
				print('Failed to dump new subclass "%s" to %s' % (subclass, (self._subclass_path + subclass + '.json')))