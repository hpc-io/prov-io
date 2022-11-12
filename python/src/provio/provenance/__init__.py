import RDF
from provio.extensible import Extensible
from provio.provenance.Provenance import Provenance


class PROVIO(Provenance):

	def add_triple(self, _subject, _predicate, _object, type = None):
		'''
		Expand an existing record with a new triple
		@type uri or value
		'''
		# Create a record for version if version is enabled and this is the first write 
		if self._first_write and self._enable_version:
			self._model.add_statement(RDF.Statement(RDF.Uri(self._version),
                    RDF.Uri('ns1:Type'),
                    RDF.Uri('Version')))
			self._first_write = False

		if type == 'value':
			# If the value is not string try to convert to string first
			_value = None
			if not isinstance(_object, str):
				try: 
					_value = str(_object)
				except:
					print('Unsupported value type')
			else: 
				_value = _object
			self._model.add_statement(RDF.Statement(RDF.Uri(_subject),
		                    RDF.Uri(_predicate),
		                    RDF.Node(_value)))
		else:
			self._model.add_statement(RDF.Statement(RDF.Uri(_subject),
                RDF.Uri(_predicate),
                RDF.Uri(_object)))
		
		self._op_count += 1

		if self._period and self._op_count % self._period == 0:
			'''
			If operation count reach the period, dump to file and load back
			'''
			self._serializer.serialize_model_to_file(self._serialize_path, self._model)
			print('Serialized %d to %d insertions to file %s' % (self._op_count - self._period,
				self._op_count, self._serialize_path))
			self._parser.parse_into_model(self._model, 'file:' + self._serialize_path, ' ')

	def new_record(self, field, **args):
		'''
		Create a new record for a field
		The field name will be combined with the uuid of this graph instance (if enabled uuid)
		The new record contains:
			(1) a triple describing its type (one of the extensible classes)
			(2) a triple of version 
			(3) a triple of value (if provided)
		'''
		_field = field
		if self._id: 
			_field = _field+'-'+self._id

		# Create a triple of type
		_type = None
		if 'type' in args:
			_type = args['type']

		else: 
			_type = Extensible(field)._subclass
		self.add_triple(_field, 'ns1:Type', _type, 'value')
		
		# Create a triple of version
		if self._enable_version:		
			self.add_triple(_field, 'ns1:belongsTo', self._version, 'uri')
	
		# Create a triple of value 
		_value = None
		if 'value' in args:
			# If the value is not string try to convert to string first
			if not isinstance(args['value'], str):
				try: 
					_value = str(args['value'])
				except:
					print('Unsupported value type')
			else:
				_value = args['value']
			self.add_triple(_field, 'ns1:hasValue', _value, 'value')


	def add_metric_to_version(self, metric, value, **args):
		'''
		Add metric to any existing version record
		Add to the current version by default
		'''
		_metric = None
		if not isinstance(metric, str):
			try: 
				_metric = str(metric) ++'-'+self._id
			except:
				print('Unsupported metric type')
		else:
			_metric = metric +'-'+self._id
		
		_value = None
		if not isinstance(value, str):
			try: 
				_value = str(value)
			except:
				print('Unsupported value type')
		else:
			_value = value

		_version = None
		if 'version' in args:
			_version = args['version']
			if _version[0] != 'v':
				_version = 'v'+_version 
		else:
			_version = self._version

		'''
		Don't implement metric version for now
		Metric version could be used to record the accuracy of each training epoch 
		and could be used to see the covergence trend
		'''
		# _metric_version = None
		# if 'metric_version' in args:
		# 	_metric_version = args['metric_version']
		# else:
		# 	_metric_version = self._metric_version

		# Create a record for version if version is enabled and this is the first write 
		# if self._first_write and self._enable_version:
		# 	self._model.add_statement(RDF.Statement(RDF.Uri(self._version),
  #                   RDF.Uri('ns1:Type'),
  #                   RDF.Uri('Version')))
		# 	self.record_insert(self._version, 'ns1:Type', Version, 'value')
		# 	self._first_write = False

		# Adding new metrics node and inserting new metric triple to record should be atomic
		# The record of a metric and the triple of a metric in a version record should be pointing to each other
		try:
			# self._model.add_statement(RDF.Statement(RDF.Uri(_metric),
   #              RDF.Uri('ns1:hasValue'),
   #              RDF.Node(_value)))
			self.add_triple(_metric, 'ns1:hasValue', _value, 'value')
			# self._model.add_statement(RDF.Statement(RDF.Uri(_metric),
   #              RDF.Uri('ns1:belongsTo'),
   #              RDF.Uri(_version)))
			self.add_triple(_metric, 'ns1:belongsTo', _version, 'uri')
			# self._model.add_statement(RDF.Statement(RDF.Uri(_version),
	  #               RDF.Uri('ns1:hasMetrics'),
	  #               RDF.Uri(_metric)))
			self.add_triple(_version, 'ns1:hasMetrics', _metric, 'uri')
		except:
			print('Failed to add metric %s to provenance %s' % (_metric, _version))
			print('Possible reasons are version does not exist or failed to create a record for the metric')


	def add_metric_to_record(self, record_subject, metric, value):
		'''
		Add metric to an existing record
		'''
		_record_subject = record_subject + '-' + self._id
		if (self._model.get_targets(RDF.Uri(_record_subject),RDF.Uri('ns1:Type'))):

			_metric = None
			if not isinstance(metric, str):
				try: 
					_metric = str(metric) +'-'+self._id
				except:
					print('Unsupported metric type')
			else:
				_metric = metric +'-'+self._id

			_value = None
			if not isinstance(value, str):
				try: 
					_value = str(value)
				except:
					print('Unsupported value type')
			else:
				_value = value
			try:
				# self._model.add_statement(RDF.Statement(RDF.Uri(_metric),
	   #              RDF.Uri('ns1:hasValue'),
	   #              RDF.Node(_value)))
				self.add_triple(_metric, 'ns1:hasValue', _value, 'value')
				# self._model.add_statement(RDF.Statement(RDF.Uri(_metric),
	   #              RDF.Uri('ns1:belongsTo'),
	   #              RDF.Uri(_record_subject)))
				self.add_triple(_metric, 'ns1:belongsTo', _record_subject, 'uri')
				# self._model.add_statement(RDF.Statement(RDF.Uri(_record_subject),
		  #               RDF.Uri('ns1:hasMetrics'),
		  #               RDF.Uri(_metric)))
				self.add_triple(_record_subject, 'ns1:hasMetrics', _metric, 'uri')
			except:
				print('Failed to add metric %s to record of %s' % (_metric, _record_subject))
			
			# Create a record for version if version is enabled and this is the first write 
			# if self._first_write and self._enable_version:
			# 	self._model.add_statement(RDF.Statement(RDF.Uri(self._version),
	  #                   RDF.Uri('ns1:Type'),
	  #                   RDF.Uri('Version')))
			# 	self._first_write = False

		else:
			print(_record_subject + ' does not exist')


	def set_prefix_and_base_uri(self, prefix, uri):
		'''
		Set the namespace (prefix) and the base uri of the provenance graph
		'''
		if (self._serializer):
			try:
				serializer.set_namespace(prefix, RDF.Uri(uri))
			except:
				raise Exception('failed to set prefix and base uri') 

	def set_serialization_path(self, path):
		self._serialize_path = path

	def serialize_prov_to_file(self, path):
		'''
		Dump the graph to disk
		'''
		self._serialize_path = path
		if (self._serializer):
			try:
				self._serializer.serialize_model_to_file(self._serialize_path, self._model)
			except:
				raise Exception('failed to serialize provenance graph to %s' % self._serialize_path) 


	def set_version(self, _version):
		'''
		For users to set their own version of the provenance instance (version of this run)
		If left unset provio will set to the default version
		'''
		self._version = RDF.Node(_version)
		print('Provenance version: v' + self._version)


	def set_identifier(self, _id):
		'''
		For users to set their own id of every record of the graph
		If left unset provid will use randomly generated uuid
		'''
		self._id = str(_id)


	# def display_all_subclasses(self):
	# 	self._subclasses.display_all_subclasses()
