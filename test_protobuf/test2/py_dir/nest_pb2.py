# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: nest.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


from google.protobuf import timestamp_pb2 as google_dot_protobuf_dot_timestamp__pb2


DESCRIPTOR = _descriptor.FileDescriptor(
  name='nest.proto',
  package='nest',
  syntax='proto3',
  serialized_options=None,
  serialized_pb=_b('\n\nnest.proto\x12\x04nest\x1a\x1fgoogle/protobuf/timestamp.proto\"\x86\x01\n\x04Task\x12\n\n\x02id\x18\x01 \x01(\t\x12\r\n\x05title\x18\x02 \x01(\t\x12\x13\n\x0b\x64\x65scription\x18\x03 \x01(\t\x12 \n\x06status\x18\x04 \x01(\x0e\x32\x10.nest.TaskStatus\x12,\n\x08\x64ue_date\x18\x05 \x01(\x0b\x32\x1a.google.protobuf.Timestamp\"%\n\x08TaskList\x12\x19\n\x05tasks\x18\x01 \x03(\x0b\x32\n.nest.Task*F\n\nTaskStatus\x12\x0b\n\x07UNKNOWN\x10\x00\x12\x0b\n\x07PENDING\x10\x01\x12\x0f\n\x0bIN_PROGRESS\x10\x02\x12\r\n\tCOMPLETED\x10\x03\x62\x06proto3')
  ,
  dependencies=[google_dot_protobuf_dot_timestamp__pb2.DESCRIPTOR,])

_TASKSTATUS = _descriptor.EnumDescriptor(
  name='TaskStatus',
  full_name='nest.TaskStatus',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='UNKNOWN', index=0, number=0,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='PENDING', index=1, number=1,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='IN_PROGRESS', index=2, number=2,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='COMPLETED', index=3, number=3,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=229,
  serialized_end=299,
)
_sym_db.RegisterEnumDescriptor(_TASKSTATUS)

TaskStatus = enum_type_wrapper.EnumTypeWrapper(_TASKSTATUS)
UNKNOWN = 0
PENDING = 1
IN_PROGRESS = 2
COMPLETED = 3



_TASK = _descriptor.Descriptor(
  name='Task',
  full_name='nest.Task',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='id', full_name='nest.Task.id', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='title', full_name='nest.Task.title', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='description', full_name='nest.Task.description', index=2,
      number=3, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='status', full_name='nest.Task.status', index=3,
      number=4, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='due_date', full_name='nest.Task.due_date', index=4,
      number=5, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=54,
  serialized_end=188,
)


_TASKLIST = _descriptor.Descriptor(
  name='TaskList',
  full_name='nest.TaskList',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='tasks', full_name='nest.TaskList.tasks', index=0,
      number=1, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=190,
  serialized_end=227,
)

_TASK.fields_by_name['status'].enum_type = _TASKSTATUS
_TASK.fields_by_name['due_date'].message_type = google_dot_protobuf_dot_timestamp__pb2._TIMESTAMP
_TASKLIST.fields_by_name['tasks'].message_type = _TASK
DESCRIPTOR.message_types_by_name['Task'] = _TASK
DESCRIPTOR.message_types_by_name['TaskList'] = _TASKLIST
DESCRIPTOR.enum_types_by_name['TaskStatus'] = _TASKSTATUS
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

Task = _reflection.GeneratedProtocolMessageType('Task', (_message.Message,), {
  'DESCRIPTOR' : _TASK,
  '__module__' : 'nest_pb2'
  # @@protoc_insertion_point(class_scope:nest.Task)
  })
_sym_db.RegisterMessage(Task)

TaskList = _reflection.GeneratedProtocolMessageType('TaskList', (_message.Message,), {
  'DESCRIPTOR' : _TASKLIST,
  '__module__' : 'nest_pb2'
  # @@protoc_insertion_point(class_scope:nest.TaskList)
  })
_sym_db.RegisterMessage(TaskList)


# @@protoc_insertion_point(module_scope)
