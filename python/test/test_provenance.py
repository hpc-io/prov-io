from provio.provenance import PROVIO
from provio.extensible import Extensible

import os

def cleanup(path):
	os.remove(path)

def main():
	'''
	Initialize a provenance instance
	'''
	print('\n++ Initialize a provenance instance: start')
	print('-------------------------------------------')
	graph = PROVIO(serializer = 'turtle', enable_id = True, set_version = 2.0)
	print('-------------------------------------------')
	print('++ Initialize a provenance instance: done\n')

	'''
	Create a new record in the provenance instance
	'''
	print('\n++ Create a new record: start')
	print('-------------------------------------------')
	hparam = Extensible('learning_rate')
	graph.new_record(hparam._field, type=hparam._subclass, value=1)
	print('-------------------------------------------')
	print('++ Create a new record: done\n')

	'''
	Add metric to this version of provenance
	'''
	print('\n++ Add metric to  version: start')
	print('-------------------------------------------')
	metric = Extensible('TrainingAccuracy')
	graph.add_metric_to_version(metric._field, 0.98)
	print('-------------------------------------------')
	print('++ Add metric to  version: done\n')

	'''
	Add metric to a record in the provenance instance
	'''
	print('\n++ Add metric to record: start')
	print('-------------------------------------------')
	graph.add_metric_to_record(hparam._field, metric._field, 0.98)
	print('-------------------------------------------')
	print('++ Add metric to record: done\n')

	'''
	Serialize to current directory
	'''
	print('\n++ Serialize to current directory: start')
	print('-------------------------------------------')
	graph.serialize_prov_to_file('./prov.turtle')
	print('-------------------------------------------')
	print('++ Serialize to current directory: done\n')

	'''
	Load provenance from file and make a copy of it
	'''
	print('\n++ Load from file and make a copy: start')
	print('-------------------------------------------')
	graph = PROVIO(serializer = 'turtle', enable_id = True, 
		load_provenance = './prov.turtle', set_version = 2.0)
	graph.serialize_prov_to_file('./copy.turtle')
	print('-------------------------------------------')
	print('++ Load from file and make a copy: done\n')

	'''
	Periodically dump to a file
	'''
	print('\n++ Periodically dump: start')
	print('-------------------------------------------')
	graph = PROVIO(serializer = 'turtle', enable_id = False, 
		set_version = 3.0, periodical = 10)
	graph.set_serialization_path('./periodical.turtle')
	graph.new_record(hparam._field)
	for i in range(0,20):
		graph.add_triple(hparam._field, 'ns1:hasValue', i, 'value')
	graph.serialize_prov_to_file('./periodical.turtle')
	print('-------------------------------------------')
	print('++ Periodically dump: done\n')

	'''
	Clean up test class
	'''
	cleanup('./prov.turtle')
	cleanup('./copy.turtle')
	cleanup('./periodical.turtle')
	print('\n++ Clean up test provenance file: done\n')


if __name__ == '__main__':
	print('+ PROVIO class test start')
	print('===========================================')
	main()
	print('===========================================')
	print('+ PROVIO class test end')
