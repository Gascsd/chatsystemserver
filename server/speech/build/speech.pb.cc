// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: speech.proto

#include "speech.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace zht_im {
class SpeechRecognitionReqDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<SpeechRecognitionReq> _instance;
} _SpeechRecognitionReq_default_instance_;
class SpeechRecognitionRspDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<SpeechRecognitionRsp> _instance;
} _SpeechRecognitionRsp_default_instance_;
}  // namespace zht_im
static void InitDefaultsscc_info_SpeechRecognitionReq_speech_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::zht_im::_SpeechRecognitionReq_default_instance_;
    new (ptr) ::zht_im::SpeechRecognitionReq();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::zht_im::SpeechRecognitionReq::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_SpeechRecognitionReq_speech_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_SpeechRecognitionReq_speech_2eproto}, {}};

static void InitDefaultsscc_info_SpeechRecognitionRsp_speech_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::zht_im::_SpeechRecognitionRsp_default_instance_;
    new (ptr) ::zht_im::SpeechRecognitionRsp();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::zht_im::SpeechRecognitionRsp::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_SpeechRecognitionRsp_speech_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_SpeechRecognitionRsp_speech_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_speech_2eproto[2];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_speech_2eproto = nullptr;
static const ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor* file_level_service_descriptors_speech_2eproto[1];

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_speech_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::zht_im::SpeechRecognitionReq, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::zht_im::SpeechRecognitionReq, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::zht_im::SpeechRecognitionReq, request_id_),
  PROTOBUF_FIELD_OFFSET(::zht_im::SpeechRecognitionReq, speech_content_),
  PROTOBUF_FIELD_OFFSET(::zht_im::SpeechRecognitionReq, user_id_),
  PROTOBUF_FIELD_OFFSET(::zht_im::SpeechRecognitionReq, session_id_),
  ~0u,
  ~0u,
  0,
  1,
  PROTOBUF_FIELD_OFFSET(::zht_im::SpeechRecognitionRsp, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::zht_im::SpeechRecognitionRsp, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::zht_im::SpeechRecognitionRsp, request_id_),
  PROTOBUF_FIELD_OFFSET(::zht_im::SpeechRecognitionRsp, success_),
  PROTOBUF_FIELD_OFFSET(::zht_im::SpeechRecognitionRsp, errmsg_),
  PROTOBUF_FIELD_OFFSET(::zht_im::SpeechRecognitionRsp, recognition_result_),
  ~0u,
  ~0u,
  0,
  1,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 9, sizeof(::zht_im::SpeechRecognitionReq)},
  { 13, 22, sizeof(::zht_im::SpeechRecognitionRsp)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::zht_im::_SpeechRecognitionReq_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::zht_im::_SpeechRecognitionRsp_default_instance_),
};

const char descriptor_table_protodef_speech_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\014speech.proto\022\006zht_im\"\214\001\n\024SpeechRecogni"
  "tionReq\022\022\n\nrequest_id\030\001 \001(\t\022\026\n\016speech_co"
  "ntent\030\002 \001(\014\022\024\n\007user_id\030\003 \001(\tH\000\210\001\001\022\027\n\nses"
  "sion_id\030\004 \001(\tH\001\210\001\001B\n\n\010_user_idB\r\n\013_sessi"
  "on_id\"\223\001\n\024SpeechRecognitionRsp\022\022\n\nreques"
  "t_id\030\001 \001(\t\022\017\n\007success\030\002 \001(\010\022\023\n\006errmsg\030\003 "
  "\001(\tH\000\210\001\001\022\037\n\022recognition_result\030\004 \001(\tH\001\210\001"
  "\001B\t\n\007_errmsgB\025\n\023_recognition_result2`\n\rS"
  "peechService\022O\n\021SpeechRecognition\022\034.zht_"
  "im.SpeechRecognitionReq\032\034.zht_im.SpeechR"
  "ecognitionRspB\003\200\001\001b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_speech_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_speech_2eproto_sccs[2] = {
  &scc_info_SpeechRecognitionReq_speech_2eproto.base,
  &scc_info_SpeechRecognitionRsp_speech_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_speech_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_speech_2eproto = {
  false, false, descriptor_table_protodef_speech_2eproto, "speech.proto", 426,
  &descriptor_table_speech_2eproto_once, descriptor_table_speech_2eproto_sccs, descriptor_table_speech_2eproto_deps, 2, 0,
  schemas, file_default_instances, TableStruct_speech_2eproto::offsets,
  file_level_metadata_speech_2eproto, 2, file_level_enum_descriptors_speech_2eproto, file_level_service_descriptors_speech_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_speech_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_speech_2eproto)), true);
namespace zht_im {

// ===================================================================

void SpeechRecognitionReq::InitAsDefaultInstance() {
}
class SpeechRecognitionReq::_Internal {
 public:
  using HasBits = decltype(std::declval<SpeechRecognitionReq>()._has_bits_);
  static void set_has_user_id(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_session_id(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
};

SpeechRecognitionReq::SpeechRecognitionReq(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:zht_im.SpeechRecognitionReq)
}
SpeechRecognitionReq::SpeechRecognitionReq(const SpeechRecognitionReq& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  request_id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_request_id().empty()) {
    request_id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from._internal_request_id(),
      GetArena());
  }
  speech_content_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_speech_content().empty()) {
    speech_content_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from._internal_speech_content(),
      GetArena());
  }
  user_id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_user_id()) {
    user_id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from._internal_user_id(),
      GetArena());
  }
  session_id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_session_id()) {
    session_id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from._internal_session_id(),
      GetArena());
  }
  // @@protoc_insertion_point(copy_constructor:zht_im.SpeechRecognitionReq)
}

void SpeechRecognitionReq::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_SpeechRecognitionReq_speech_2eproto.base);
  request_id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  speech_content_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  user_id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  session_id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

SpeechRecognitionReq::~SpeechRecognitionReq() {
  // @@protoc_insertion_point(destructor:zht_im.SpeechRecognitionReq)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void SpeechRecognitionReq::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  request_id_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  speech_content_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  user_id_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  session_id_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void SpeechRecognitionReq::ArenaDtor(void* object) {
  SpeechRecognitionReq* _this = reinterpret_cast< SpeechRecognitionReq* >(object);
  (void)_this;
}
void SpeechRecognitionReq::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void SpeechRecognitionReq::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const SpeechRecognitionReq& SpeechRecognitionReq::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_SpeechRecognitionReq_speech_2eproto.base);
  return *internal_default_instance();
}


void SpeechRecognitionReq::Clear() {
// @@protoc_insertion_point(message_clear_start:zht_im.SpeechRecognitionReq)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  request_id_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  speech_content_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      user_id_.ClearNonDefaultToEmpty();
    }
    if (cached_has_bits & 0x00000002u) {
      session_id_.ClearNonDefaultToEmpty();
    }
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* SpeechRecognitionReq::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // string request_id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          auto str = _internal_mutable_request_id();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "zht_im.SpeechRecognitionReq.request_id"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // bytes speech_content = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          auto str = _internal_mutable_speech_content();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string user_id = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          auto str = _internal_mutable_user_id();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "zht_im.SpeechRecognitionReq.user_id"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string session_id = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          auto str = _internal_mutable_session_id();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "zht_im.SpeechRecognitionReq.session_id"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* SpeechRecognitionReq::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:zht_im.SpeechRecognitionReq)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string request_id = 1;
  if (this->request_id().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_request_id().data(), static_cast<int>(this->_internal_request_id().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "zht_im.SpeechRecognitionReq.request_id");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_request_id(), target);
  }

  // bytes speech_content = 2;
  if (this->speech_content().size() > 0) {
    target = stream->WriteBytesMaybeAliased(
        2, this->_internal_speech_content(), target);
  }

  // string user_id = 3;
  if (_internal_has_user_id()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_user_id().data(), static_cast<int>(this->_internal_user_id().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "zht_im.SpeechRecognitionReq.user_id");
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_user_id(), target);
  }

  // string session_id = 4;
  if (_internal_has_session_id()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_session_id().data(), static_cast<int>(this->_internal_session_id().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "zht_im.SpeechRecognitionReq.session_id");
    target = stream->WriteStringMaybeAliased(
        4, this->_internal_session_id(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:zht_im.SpeechRecognitionReq)
  return target;
}

size_t SpeechRecognitionReq::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:zht_im.SpeechRecognitionReq)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string request_id = 1;
  if (this->request_id().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_request_id());
  }

  // bytes speech_content = 2;
  if (this->speech_content().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_speech_content());
  }

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    // string user_id = 3;
    if (cached_has_bits & 0x00000001u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_user_id());
    }

    // string session_id = 4;
    if (cached_has_bits & 0x00000002u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_session_id());
    }

  }
  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void SpeechRecognitionReq::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:zht_im.SpeechRecognitionReq)
  GOOGLE_DCHECK_NE(&from, this);
  const SpeechRecognitionReq* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<SpeechRecognitionReq>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:zht_im.SpeechRecognitionReq)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:zht_im.SpeechRecognitionReq)
    MergeFrom(*source);
  }
}

void SpeechRecognitionReq::MergeFrom(const SpeechRecognitionReq& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:zht_im.SpeechRecognitionReq)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.request_id().size() > 0) {
    _internal_set_request_id(from._internal_request_id());
  }
  if (from.speech_content().size() > 0) {
    _internal_set_speech_content(from._internal_speech_content());
  }
  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      _internal_set_user_id(from._internal_user_id());
    }
    if (cached_has_bits & 0x00000002u) {
      _internal_set_session_id(from._internal_session_id());
    }
  }
}

void SpeechRecognitionReq::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:zht_im.SpeechRecognitionReq)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SpeechRecognitionReq::CopyFrom(const SpeechRecognitionReq& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:zht_im.SpeechRecognitionReq)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SpeechRecognitionReq::IsInitialized() const {
  return true;
}

void SpeechRecognitionReq::InternalSwap(SpeechRecognitionReq* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  request_id_.Swap(&other->request_id_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  speech_content_.Swap(&other->speech_content_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  user_id_.Swap(&other->user_id_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  session_id_.Swap(&other->session_id_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}

::PROTOBUF_NAMESPACE_ID::Metadata SpeechRecognitionReq::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void SpeechRecognitionRsp::InitAsDefaultInstance() {
}
class SpeechRecognitionRsp::_Internal {
 public:
  using HasBits = decltype(std::declval<SpeechRecognitionRsp>()._has_bits_);
  static void set_has_errmsg(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_recognition_result(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
};

SpeechRecognitionRsp::SpeechRecognitionRsp(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:zht_im.SpeechRecognitionRsp)
}
SpeechRecognitionRsp::SpeechRecognitionRsp(const SpeechRecognitionRsp& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  request_id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_request_id().empty()) {
    request_id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from._internal_request_id(),
      GetArena());
  }
  errmsg_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_errmsg()) {
    errmsg_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from._internal_errmsg(),
      GetArena());
  }
  recognition_result_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_recognition_result()) {
    recognition_result_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from._internal_recognition_result(),
      GetArena());
  }
  success_ = from.success_;
  // @@protoc_insertion_point(copy_constructor:zht_im.SpeechRecognitionRsp)
}

void SpeechRecognitionRsp::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_SpeechRecognitionRsp_speech_2eproto.base);
  request_id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  errmsg_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  recognition_result_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  success_ = false;
}

SpeechRecognitionRsp::~SpeechRecognitionRsp() {
  // @@protoc_insertion_point(destructor:zht_im.SpeechRecognitionRsp)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void SpeechRecognitionRsp::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  request_id_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  errmsg_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  recognition_result_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void SpeechRecognitionRsp::ArenaDtor(void* object) {
  SpeechRecognitionRsp* _this = reinterpret_cast< SpeechRecognitionRsp* >(object);
  (void)_this;
}
void SpeechRecognitionRsp::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void SpeechRecognitionRsp::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const SpeechRecognitionRsp& SpeechRecognitionRsp::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_SpeechRecognitionRsp_speech_2eproto.base);
  return *internal_default_instance();
}


void SpeechRecognitionRsp::Clear() {
// @@protoc_insertion_point(message_clear_start:zht_im.SpeechRecognitionRsp)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  request_id_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      errmsg_.ClearNonDefaultToEmpty();
    }
    if (cached_has_bits & 0x00000002u) {
      recognition_result_.ClearNonDefaultToEmpty();
    }
  }
  success_ = false;
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* SpeechRecognitionRsp::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // string request_id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          auto str = _internal_mutable_request_id();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "zht_im.SpeechRecognitionRsp.request_id"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // bool success = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          success_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string errmsg = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          auto str = _internal_mutable_errmsg();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "zht_im.SpeechRecognitionRsp.errmsg"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string recognition_result = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          auto str = _internal_mutable_recognition_result();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "zht_im.SpeechRecognitionRsp.recognition_result"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* SpeechRecognitionRsp::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:zht_im.SpeechRecognitionRsp)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string request_id = 1;
  if (this->request_id().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_request_id().data(), static_cast<int>(this->_internal_request_id().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "zht_im.SpeechRecognitionRsp.request_id");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_request_id(), target);
  }

  // bool success = 2;
  if (this->success() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(2, this->_internal_success(), target);
  }

  // string errmsg = 3;
  if (_internal_has_errmsg()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_errmsg().data(), static_cast<int>(this->_internal_errmsg().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "zht_im.SpeechRecognitionRsp.errmsg");
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_errmsg(), target);
  }

  // string recognition_result = 4;
  if (_internal_has_recognition_result()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_recognition_result().data(), static_cast<int>(this->_internal_recognition_result().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "zht_im.SpeechRecognitionRsp.recognition_result");
    target = stream->WriteStringMaybeAliased(
        4, this->_internal_recognition_result(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:zht_im.SpeechRecognitionRsp)
  return target;
}

size_t SpeechRecognitionRsp::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:zht_im.SpeechRecognitionRsp)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string request_id = 1;
  if (this->request_id().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_request_id());
  }

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    // string errmsg = 3;
    if (cached_has_bits & 0x00000001u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_errmsg());
    }

    // string recognition_result = 4;
    if (cached_has_bits & 0x00000002u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_recognition_result());
    }

  }
  // bool success = 2;
  if (this->success() != 0) {
    total_size += 1 + 1;
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void SpeechRecognitionRsp::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:zht_im.SpeechRecognitionRsp)
  GOOGLE_DCHECK_NE(&from, this);
  const SpeechRecognitionRsp* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<SpeechRecognitionRsp>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:zht_im.SpeechRecognitionRsp)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:zht_im.SpeechRecognitionRsp)
    MergeFrom(*source);
  }
}

void SpeechRecognitionRsp::MergeFrom(const SpeechRecognitionRsp& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:zht_im.SpeechRecognitionRsp)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.request_id().size() > 0) {
    _internal_set_request_id(from._internal_request_id());
  }
  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      _internal_set_errmsg(from._internal_errmsg());
    }
    if (cached_has_bits & 0x00000002u) {
      _internal_set_recognition_result(from._internal_recognition_result());
    }
  }
  if (from.success() != 0) {
    _internal_set_success(from._internal_success());
  }
}

void SpeechRecognitionRsp::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:zht_im.SpeechRecognitionRsp)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SpeechRecognitionRsp::CopyFrom(const SpeechRecognitionRsp& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:zht_im.SpeechRecognitionRsp)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SpeechRecognitionRsp::IsInitialized() const {
  return true;
}

void SpeechRecognitionRsp::InternalSwap(SpeechRecognitionRsp* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  request_id_.Swap(&other->request_id_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  errmsg_.Swap(&other->errmsg_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  recognition_result_.Swap(&other->recognition_result_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  swap(success_, other->success_);
}

::PROTOBUF_NAMESPACE_ID::Metadata SpeechRecognitionRsp::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

SpeechService::~SpeechService() {}

const ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor* SpeechService::descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_speech_2eproto);
  return file_level_service_descriptors_speech_2eproto[0];
}

const ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor* SpeechService::GetDescriptor() {
  return descriptor();
}

void SpeechService::SpeechRecognition(::PROTOBUF_NAMESPACE_ID::RpcController* controller,
                         const ::zht_im::SpeechRecognitionReq*,
                         ::zht_im::SpeechRecognitionRsp*,
                         ::google::protobuf::Closure* done) {
  controller->SetFailed("Method SpeechRecognition() not implemented.");
  done->Run();
}

void SpeechService::CallMethod(const ::PROTOBUF_NAMESPACE_ID::MethodDescriptor* method,
                             ::PROTOBUF_NAMESPACE_ID::RpcController* controller,
                             const ::PROTOBUF_NAMESPACE_ID::Message* request,
                             ::PROTOBUF_NAMESPACE_ID::Message* response,
                             ::google::protobuf::Closure* done) {
  GOOGLE_DCHECK_EQ(method->service(), file_level_service_descriptors_speech_2eproto[0]);
  switch(method->index()) {
    case 0:
      SpeechRecognition(controller,
             ::PROTOBUF_NAMESPACE_ID::internal::DownCast<const ::zht_im::SpeechRecognitionReq*>(
                 request),
             ::PROTOBUF_NAMESPACE_ID::internal::DownCast<::zht_im::SpeechRecognitionRsp*>(
                 response),
             done);
      break;
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      break;
  }
}

const ::PROTOBUF_NAMESPACE_ID::Message& SpeechService::GetRequestPrototype(
    const ::PROTOBUF_NAMESPACE_ID::MethodDescriptor* method) const {
  GOOGLE_DCHECK_EQ(method->service(), descriptor());
  switch(method->index()) {
    case 0:
      return ::zht_im::SpeechRecognitionReq::default_instance();
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      return *::PROTOBUF_NAMESPACE_ID::MessageFactory::generated_factory()
          ->GetPrototype(method->input_type());
  }
}

const ::PROTOBUF_NAMESPACE_ID::Message& SpeechService::GetResponsePrototype(
    const ::PROTOBUF_NAMESPACE_ID::MethodDescriptor* method) const {
  GOOGLE_DCHECK_EQ(method->service(), descriptor());
  switch(method->index()) {
    case 0:
      return ::zht_im::SpeechRecognitionRsp::default_instance();
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      return *::PROTOBUF_NAMESPACE_ID::MessageFactory::generated_factory()
          ->GetPrototype(method->output_type());
  }
}

SpeechService_Stub::SpeechService_Stub(::PROTOBUF_NAMESPACE_ID::RpcChannel* channel)
  : channel_(channel), owns_channel_(false) {}
SpeechService_Stub::SpeechService_Stub(
    ::PROTOBUF_NAMESPACE_ID::RpcChannel* channel,
    ::PROTOBUF_NAMESPACE_ID::Service::ChannelOwnership ownership)
  : channel_(channel),
    owns_channel_(ownership == ::PROTOBUF_NAMESPACE_ID::Service::STUB_OWNS_CHANNEL) {}
SpeechService_Stub::~SpeechService_Stub() {
  if (owns_channel_) delete channel_;
}

void SpeechService_Stub::SpeechRecognition(::PROTOBUF_NAMESPACE_ID::RpcController* controller,
                              const ::zht_im::SpeechRecognitionReq* request,
                              ::zht_im::SpeechRecognitionRsp* response,
                              ::google::protobuf::Closure* done) {
  channel_->CallMethod(descriptor()->method(0),
                       controller, request, response, done);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace zht_im
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::zht_im::SpeechRecognitionReq* Arena::CreateMaybeMessage< ::zht_im::SpeechRecognitionReq >(Arena* arena) {
  return Arena::CreateMessageInternal< ::zht_im::SpeechRecognitionReq >(arena);
}
template<> PROTOBUF_NOINLINE ::zht_im::SpeechRecognitionRsp* Arena::CreateMaybeMessage< ::zht_im::SpeechRecognitionRsp >(Arena* arena) {
  return Arena::CreateMessageInternal< ::zht_im::SpeechRecognitionRsp >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>