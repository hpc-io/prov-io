from provio.extensible import Extensible
import os

def cleanup(path):
	os.remove(path)

def main():
	'''
	Empty parameter
	'''
	print('\n++ Empty parameter: start')
	print('-------------------------------------------')
	extensible = Extensible()
	print(extensible._field, extensible._subclass)
	print('-------------------------------------------')
	print('++ Empty parameter: done\n')

	'''
	Field name in parameter
	'''
	print('\n++ Field name in parameter: start')
	print('-------------------------------------------')
	extensible = Extensible('a')
	print(extensible._field, extensible._subclass)
	extensible = Extensible('Hyperparameters')
	print(extensible._field, extensible._subclass)
	print('-------------------------------------------')
	print('++ Field name in parameter: done\n')

	'''
	Display fields in a subclass
	'''
	print('\n++ Display fields in a subclass: start')
	print('-------------------------------------------')
	extensible.display_subclass_fields('Configuration')
	print('-------------------------------------------')
	print('++ Display fields in a subclass: done\n')

	'''
	Display fields in all subclasses
	'''
	print('\n++ Display fields in all subclasses: start')
	print('-------------------------------------------')
	extensible.display_all_subclasses()
	print('-------------------------------------------')
	print('++ Display fields in all subclasses: done\n')

	'''
	Add new a subclass and fields
	'''
	print('\n++ Add new a subclass and fields: start')
	print('-------------------------------------------')
	extensible.display_subclass_fields('test_subclass')
	extensible.add_subclass('test_class','test_subclass', 'test_field_0')
	test_fields = ['test_field_1', 'test_field_2']
	extensible.add_subclass('test_class', 'test_subclass', test_fields)
	extensible.add_subclass('test_class', 'test_subclass', test_fields)
	extensible.display_subclass_fields('test_subclass')
	print('-------------------------------------------')
	print('++ Add new a subclass and fields: done\n')

	'''
	Clean up test class
	'''
	cleanup(extensible._subclass_path + 'test_class' + '.json')
	print('\n++ Clean up test class: done\n')

if __name__ == '__main__':
	print('+ Extensible class test start')
	print('===========================================')
	main()
	print('===========================================')
	print('+ Extensible class test end')
