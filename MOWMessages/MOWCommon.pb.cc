// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: MOWCommon.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "MOWCommon.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace PbMOWCommon {

namespace {

const ::google::protobuf::Descriptor* PbMOWVector3_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  PbMOWVector3_reflection_ = NULL;
const ::google::protobuf::Descriptor* PbMOWVector4_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  PbMOWVector4_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_MOWCommon_2eproto() {
  protobuf_AddDesc_MOWCommon_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "MOWCommon.proto");
  GOOGLE_CHECK(file != NULL);
  PbMOWVector3_descriptor_ = file->message_type(0);
  static const int PbMOWVector3_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PbMOWVector3, x_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PbMOWVector3, y_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PbMOWVector3, z_),
  };
  PbMOWVector3_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      PbMOWVector3_descriptor_,
      PbMOWVector3::default_instance_,
      PbMOWVector3_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PbMOWVector3, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PbMOWVector3, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(PbMOWVector3));
  PbMOWVector4_descriptor_ = file->message_type(1);
  static const int PbMOWVector4_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PbMOWVector4, x_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PbMOWVector4, y_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PbMOWVector4, z_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PbMOWVector4, w_),
  };
  PbMOWVector4_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      PbMOWVector4_descriptor_,
      PbMOWVector4::default_instance_,
      PbMOWVector4_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PbMOWVector4, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PbMOWVector4, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(PbMOWVector4));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_MOWCommon_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    PbMOWVector3_descriptor_, &PbMOWVector3::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    PbMOWVector4_descriptor_, &PbMOWVector4::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_MOWCommon_2eproto() {
  delete PbMOWVector3::default_instance_;
  delete PbMOWVector3_reflection_;
  delete PbMOWVector4::default_instance_;
  delete PbMOWVector4_reflection_;
}

void protobuf_AddDesc_MOWCommon_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\017MOWCommon.proto\022\013PbMOWCommon\"/\n\014PbMOWV"
    "ector3\022\t\n\001x\030\001 \001(\002\022\t\n\001y\030\002 \001(\002\022\t\n\001z\030\003 \001(\002\""
    ":\n\014PbMOWVector4\022\t\n\001x\030\001 \001(\001\022\t\n\001y\030\002 \001(\001\022\t\n"
    "\001z\030\003 \001(\001\022\t\n\001w\030\004 \001(\001", 139);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "MOWCommon.proto", &protobuf_RegisterTypes);
  PbMOWVector3::default_instance_ = new PbMOWVector3();
  PbMOWVector4::default_instance_ = new PbMOWVector4();
  PbMOWVector3::default_instance_->InitAsDefaultInstance();
  PbMOWVector4::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_MOWCommon_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_MOWCommon_2eproto {
  StaticDescriptorInitializer_MOWCommon_2eproto() {
    protobuf_AddDesc_MOWCommon_2eproto();
  }
} static_descriptor_initializer_MOWCommon_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int PbMOWVector3::kXFieldNumber;
const int PbMOWVector3::kYFieldNumber;
const int PbMOWVector3::kZFieldNumber;
#endif  // !_MSC_VER

PbMOWVector3::PbMOWVector3()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:PbMOWCommon.PbMOWVector3)
}

void PbMOWVector3::InitAsDefaultInstance() {
}

PbMOWVector3::PbMOWVector3(const PbMOWVector3& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:PbMOWCommon.PbMOWVector3)
}

void PbMOWVector3::SharedCtor() {
  _cached_size_ = 0;
  x_ = 0;
  y_ = 0;
  z_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

PbMOWVector3::~PbMOWVector3() {
  // @@protoc_insertion_point(destructor:PbMOWCommon.PbMOWVector3)
  SharedDtor();
}

void PbMOWVector3::SharedDtor() {
  if (this != default_instance_) {
  }
}

void PbMOWVector3::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* PbMOWVector3::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return PbMOWVector3_descriptor_;
}

const PbMOWVector3& PbMOWVector3::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_MOWCommon_2eproto();
  return *default_instance_;
}

PbMOWVector3* PbMOWVector3::default_instance_ = NULL;

PbMOWVector3* PbMOWVector3::New() const {
  return new PbMOWVector3;
}

void PbMOWVector3::Clear() {
#define OFFSET_OF_FIELD_(f) (reinterpret_cast<char*>(      \
  &reinterpret_cast<PbMOWVector3*>(16)->f) - \
   reinterpret_cast<char*>(16))

#define ZR_(first, last) do {                              \
    size_t f = OFFSET_OF_FIELD_(first);                    \
    size_t n = OFFSET_OF_FIELD_(last) - f + sizeof(last);  \
    ::memset(&first, 0, n);                                \
  } while (0)

  ZR_(x_, z_);

#undef OFFSET_OF_FIELD_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool PbMOWVector3::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:PbMOWCommon.PbMOWVector3)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional float x = 1;
      case 1: {
        if (tag == 13) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &x_)));
          set_has_x();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(21)) goto parse_y;
        break;
      }

      // optional float y = 2;
      case 2: {
        if (tag == 21) {
         parse_y:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &y_)));
          set_has_y();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(29)) goto parse_z;
        break;
      }

      // optional float z = 3;
      case 3: {
        if (tag == 29) {
         parse_z:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &z_)));
          set_has_z();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:PbMOWCommon.PbMOWVector3)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:PbMOWCommon.PbMOWVector3)
  return false;
#undef DO_
}

void PbMOWVector3::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:PbMOWCommon.PbMOWVector3)
  // optional float x = 1;
  if (has_x()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(1, this->x(), output);
  }

  // optional float y = 2;
  if (has_y()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(2, this->y(), output);
  }

  // optional float z = 3;
  if (has_z()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(3, this->z(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:PbMOWCommon.PbMOWVector3)
}

::google::protobuf::uint8* PbMOWVector3::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:PbMOWCommon.PbMOWVector3)
  // optional float x = 1;
  if (has_x()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(1, this->x(), target);
  }

  // optional float y = 2;
  if (has_y()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(2, this->y(), target);
  }

  // optional float z = 3;
  if (has_z()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(3, this->z(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:PbMOWCommon.PbMOWVector3)
  return target;
}

int PbMOWVector3::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional float x = 1;
    if (has_x()) {
      total_size += 1 + 4;
    }

    // optional float y = 2;
    if (has_y()) {
      total_size += 1 + 4;
    }

    // optional float z = 3;
    if (has_z()) {
      total_size += 1 + 4;
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void PbMOWVector3::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const PbMOWVector3* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const PbMOWVector3*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void PbMOWVector3::MergeFrom(const PbMOWVector3& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_x()) {
      set_x(from.x());
    }
    if (from.has_y()) {
      set_y(from.y());
    }
    if (from.has_z()) {
      set_z(from.z());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void PbMOWVector3::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void PbMOWVector3::CopyFrom(const PbMOWVector3& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool PbMOWVector3::IsInitialized() const {

  return true;
}

void PbMOWVector3::Swap(PbMOWVector3* other) {
  if (other != this) {
    std::swap(x_, other->x_);
    std::swap(y_, other->y_);
    std::swap(z_, other->z_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata PbMOWVector3::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = PbMOWVector3_descriptor_;
  metadata.reflection = PbMOWVector3_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int PbMOWVector4::kXFieldNumber;
const int PbMOWVector4::kYFieldNumber;
const int PbMOWVector4::kZFieldNumber;
const int PbMOWVector4::kWFieldNumber;
#endif  // !_MSC_VER

PbMOWVector4::PbMOWVector4()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:PbMOWCommon.PbMOWVector4)
}

void PbMOWVector4::InitAsDefaultInstance() {
}

PbMOWVector4::PbMOWVector4(const PbMOWVector4& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:PbMOWCommon.PbMOWVector4)
}

void PbMOWVector4::SharedCtor() {
  _cached_size_ = 0;
  x_ = 0;
  y_ = 0;
  z_ = 0;
  w_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

PbMOWVector4::~PbMOWVector4() {
  // @@protoc_insertion_point(destructor:PbMOWCommon.PbMOWVector4)
  SharedDtor();
}

void PbMOWVector4::SharedDtor() {
  if (this != default_instance_) {
  }
}

void PbMOWVector4::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* PbMOWVector4::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return PbMOWVector4_descriptor_;
}

const PbMOWVector4& PbMOWVector4::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_MOWCommon_2eproto();
  return *default_instance_;
}

PbMOWVector4* PbMOWVector4::default_instance_ = NULL;

PbMOWVector4* PbMOWVector4::New() const {
  return new PbMOWVector4;
}

void PbMOWVector4::Clear() {
#define OFFSET_OF_FIELD_(f) (reinterpret_cast<char*>(      \
  &reinterpret_cast<PbMOWVector4*>(16)->f) - \
   reinterpret_cast<char*>(16))

#define ZR_(first, last) do {                              \
    size_t f = OFFSET_OF_FIELD_(first);                    \
    size_t n = OFFSET_OF_FIELD_(last) - f + sizeof(last);  \
    ::memset(&first, 0, n);                                \
  } while (0)

  ZR_(x_, w_);

#undef OFFSET_OF_FIELD_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool PbMOWVector4::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:PbMOWCommon.PbMOWVector4)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional double x = 1;
      case 1: {
        if (tag == 9) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &x_)));
          set_has_x();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(17)) goto parse_y;
        break;
      }

      // optional double y = 2;
      case 2: {
        if (tag == 17) {
         parse_y:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &y_)));
          set_has_y();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(25)) goto parse_z;
        break;
      }

      // optional double z = 3;
      case 3: {
        if (tag == 25) {
         parse_z:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &z_)));
          set_has_z();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(33)) goto parse_w;
        break;
      }

      // optional double w = 4;
      case 4: {
        if (tag == 33) {
         parse_w:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &w_)));
          set_has_w();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:PbMOWCommon.PbMOWVector4)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:PbMOWCommon.PbMOWVector4)
  return false;
#undef DO_
}

void PbMOWVector4::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:PbMOWCommon.PbMOWVector4)
  // optional double x = 1;
  if (has_x()) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(1, this->x(), output);
  }

  // optional double y = 2;
  if (has_y()) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(2, this->y(), output);
  }

  // optional double z = 3;
  if (has_z()) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(3, this->z(), output);
  }

  // optional double w = 4;
  if (has_w()) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(4, this->w(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:PbMOWCommon.PbMOWVector4)
}

::google::protobuf::uint8* PbMOWVector4::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:PbMOWCommon.PbMOWVector4)
  // optional double x = 1;
  if (has_x()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(1, this->x(), target);
  }

  // optional double y = 2;
  if (has_y()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(2, this->y(), target);
  }

  // optional double z = 3;
  if (has_z()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(3, this->z(), target);
  }

  // optional double w = 4;
  if (has_w()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(4, this->w(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:PbMOWCommon.PbMOWVector4)
  return target;
}

int PbMOWVector4::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional double x = 1;
    if (has_x()) {
      total_size += 1 + 8;
    }

    // optional double y = 2;
    if (has_y()) {
      total_size += 1 + 8;
    }

    // optional double z = 3;
    if (has_z()) {
      total_size += 1 + 8;
    }

    // optional double w = 4;
    if (has_w()) {
      total_size += 1 + 8;
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void PbMOWVector4::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const PbMOWVector4* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const PbMOWVector4*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void PbMOWVector4::MergeFrom(const PbMOWVector4& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_x()) {
      set_x(from.x());
    }
    if (from.has_y()) {
      set_y(from.y());
    }
    if (from.has_z()) {
      set_z(from.z());
    }
    if (from.has_w()) {
      set_w(from.w());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void PbMOWVector4::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void PbMOWVector4::CopyFrom(const PbMOWVector4& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool PbMOWVector4::IsInitialized() const {

  return true;
}

void PbMOWVector4::Swap(PbMOWVector4* other) {
  if (other != this) {
    std::swap(x_, other->x_);
    std::swap(y_, other->y_);
    std::swap(z_, other->z_);
    std::swap(w_, other->w_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata PbMOWVector4::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = PbMOWVector4_descriptor_;
  metadata.reflection = PbMOWVector4_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace PbMOWCommon

// @@protoc_insertion_point(global_scope)