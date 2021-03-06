// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: poker.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "poker.pb.h"

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

namespace {

const ::google::protobuf::Descriptor* card_info_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  card_info_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* e_table_type_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_poker_2eproto() {
  protobuf_AddDesc_poker_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "poker.proto");
  GOOGLE_CHECK(file != NULL);
  card_info_descriptor_ = file->message_type(0);
  static const int card_info_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(card_info, card_type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(card_info, data_),
  };
  card_info_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      card_info_descriptor_,
      card_info::default_instance_,
      card_info_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(card_info, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(card_info, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(card_info));
  e_table_type_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_poker_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    card_info_descriptor_, &card_info::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_poker_2eproto() {
  delete card_info::default_instance_;
  delete card_info_reflection_;
}

void protobuf_AddDesc_poker_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\013poker.proto\",\n\tcard_info\022\021\n\tcard_type\030"
    "\001 \002(\005\022\014\n\004data\030\002 \002(\014*&\n\014e_table_type\022\026\n\022e"
    "_classic_doudizhu\020\001", 99);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "poker.proto", &protobuf_RegisterTypes);
  card_info::default_instance_ = new card_info();
  card_info::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_poker_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_poker_2eproto {
  StaticDescriptorInitializer_poker_2eproto() {
    protobuf_AddDesc_poker_2eproto();
  }
} static_descriptor_initializer_poker_2eproto_;
const ::google::protobuf::EnumDescriptor* e_table_type_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return e_table_type_descriptor_;
}
bool e_table_type_IsValid(int value) {
  switch(value) {
    case 1:
      return true;
    default:
      return false;
  }
}


// ===================================================================

#ifndef _MSC_VER
const int card_info::kCardTypeFieldNumber;
const int card_info::kDataFieldNumber;
#endif  // !_MSC_VER

card_info::card_info()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void card_info::InitAsDefaultInstance() {
}

card_info::card_info(const card_info& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void card_info::SharedCtor() {
  _cached_size_ = 0;
  card_type_ = 0;
  data_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

card_info::~card_info() {
  SharedDtor();
}

void card_info::SharedDtor() {
  if (data_ != &::google::protobuf::internal::kEmptyString) {
    delete data_;
  }
  if (this != default_instance_) {
  }
}

void card_info::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* card_info::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return card_info_descriptor_;
}

const card_info& card_info::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_poker_2eproto();
  return *default_instance_;
}

card_info* card_info::default_instance_ = NULL;

card_info* card_info::New() const {
  return new card_info;
}

void card_info::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    card_type_ = 0;
    if (has_data()) {
      if (data_ != &::google::protobuf::internal::kEmptyString) {
        data_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool card_info::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 card_type = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &card_type_)));
          set_has_card_type();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_data;
        break;
      }

      // required bytes data = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_data:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_data()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void card_info::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 card_type = 1;
  if (has_card_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->card_type(), output);
  }

  // required bytes data = 2;
  if (has_data()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      2, this->data(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* card_info::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 card_type = 1;
  if (has_card_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->card_type(), target);
  }

  // required bytes data = 2;
  if (has_data()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        2, this->data(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int card_info::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 card_type = 1;
    if (has_card_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->card_type());
    }

    // required bytes data = 2;
    if (has_data()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->data());
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

void card_info::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const card_info* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const card_info*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void card_info::MergeFrom(const card_info& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_card_type()) {
      set_card_type(from.card_type());
    }
    if (from.has_data()) {
      set_data(from.data());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void card_info::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void card_info::CopyFrom(const card_info& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool card_info::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void card_info::Swap(card_info* other) {
  if (other != this) {
    std::swap(card_type_, other->card_type_);
    std::swap(data_, other->data_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata card_info::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = card_info_descriptor_;
  metadata.reflection = card_info_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
