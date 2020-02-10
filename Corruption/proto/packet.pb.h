// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: packet.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_packet_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_packet_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3011000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3011002 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_packet_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_packet_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_packet_2eproto;
namespace image {
class Image;
class ImageDefaultTypeInternal;
extern ImageDefaultTypeInternal _Image_default_instance_;
}  // namespace image
PROTOBUF_NAMESPACE_OPEN
template<> ::image::Image* Arena::CreateMaybeMessage<::image::Image>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace image {

// ===================================================================

class Image :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:image.Image) */ {
 public:
  Image();
  virtual ~Image();

  Image(const Image& from);
  Image(Image&& from) noexcept
    : Image() {
    *this = ::std::move(from);
  }

  inline Image& operator=(const Image& from) {
    CopyFrom(from);
    return *this;
  }
  inline Image& operator=(Image&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const Image& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Image* internal_default_instance() {
    return reinterpret_cast<const Image*>(
               &_Image_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Image& a, Image& b) {
    a.Swap(&b);
  }
  inline void Swap(Image* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Image* New() const final {
    return CreateMaybeMessage<Image>(nullptr);
  }

  Image* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Image>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Image& from);
  void MergeFrom(const Image& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Image* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "image.Image";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_packet_2eproto);
    return ::descriptor_table_packet_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kMatDataFieldNumber = 5,
    kRowsFieldNumber = 1,
    kColsFieldNumber = 2,
    kEltTypeFieldNumber = 3,
    kEltSizeFieldNumber = 4,
  };
  // bytes mat_data = 5;
  void clear_mat_data();
  const std::string& mat_data() const;
  void set_mat_data(const std::string& value);
  void set_mat_data(std::string&& value);
  void set_mat_data(const char* value);
  void set_mat_data(const void* value, size_t size);
  std::string* mutable_mat_data();
  std::string* release_mat_data();
  void set_allocated_mat_data(std::string* mat_data);
  private:
  const std::string& _internal_mat_data() const;
  void _internal_set_mat_data(const std::string& value);
  std::string* _internal_mutable_mat_data();
  public:

  // int32 rows = 1;
  void clear_rows();
  ::PROTOBUF_NAMESPACE_ID::int32 rows() const;
  void set_rows(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_rows() const;
  void _internal_set_rows(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 cols = 2;
  void clear_cols();
  ::PROTOBUF_NAMESPACE_ID::int32 cols() const;
  void set_cols(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_cols() const;
  void _internal_set_cols(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 elt_type = 3;
  void clear_elt_type();
  ::PROTOBUF_NAMESPACE_ID::int32 elt_type() const;
  void set_elt_type(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_elt_type() const;
  void _internal_set_elt_type(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 elt_size = 4;
  void clear_elt_size();
  ::PROTOBUF_NAMESPACE_ID::int32 elt_size() const;
  void set_elt_size(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_elt_size() const;
  void _internal_set_elt_size(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:image.Image)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr mat_data_;
  ::PROTOBUF_NAMESPACE_ID::int32 rows_;
  ::PROTOBUF_NAMESPACE_ID::int32 cols_;
  ::PROTOBUF_NAMESPACE_ID::int32 elt_type_;
  ::PROTOBUF_NAMESPACE_ID::int32 elt_size_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_packet_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Image

// int32 rows = 1;
inline void Image::clear_rows() {
  rows_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Image::_internal_rows() const {
  return rows_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Image::rows() const {
  // @@protoc_insertion_point(field_get:image.Image.rows)
  return _internal_rows();
}
inline void Image::_internal_set_rows(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  rows_ = value;
}
inline void Image::set_rows(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_rows(value);
  // @@protoc_insertion_point(field_set:image.Image.rows)
}

// int32 cols = 2;
inline void Image::clear_cols() {
  cols_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Image::_internal_cols() const {
  return cols_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Image::cols() const {
  // @@protoc_insertion_point(field_get:image.Image.cols)
  return _internal_cols();
}
inline void Image::_internal_set_cols(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  cols_ = value;
}
inline void Image::set_cols(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_cols(value);
  // @@protoc_insertion_point(field_set:image.Image.cols)
}

// int32 elt_type = 3;
inline void Image::clear_elt_type() {
  elt_type_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Image::_internal_elt_type() const {
  return elt_type_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Image::elt_type() const {
  // @@protoc_insertion_point(field_get:image.Image.elt_type)
  return _internal_elt_type();
}
inline void Image::_internal_set_elt_type(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  elt_type_ = value;
}
inline void Image::set_elt_type(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_elt_type(value);
  // @@protoc_insertion_point(field_set:image.Image.elt_type)
}

// int32 elt_size = 4;
inline void Image::clear_elt_size() {
  elt_size_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Image::_internal_elt_size() const {
  return elt_size_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Image::elt_size() const {
  // @@protoc_insertion_point(field_get:image.Image.elt_size)
  return _internal_elt_size();
}
inline void Image::_internal_set_elt_size(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  elt_size_ = value;
}
inline void Image::set_elt_size(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_elt_size(value);
  // @@protoc_insertion_point(field_set:image.Image.elt_size)
}

// bytes mat_data = 5;
inline void Image::clear_mat_data() {
  mat_data_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& Image::mat_data() const {
  // @@protoc_insertion_point(field_get:image.Image.mat_data)
  return _internal_mat_data();
}
inline void Image::set_mat_data(const std::string& value) {
  _internal_set_mat_data(value);
  // @@protoc_insertion_point(field_set:image.Image.mat_data)
}
inline std::string* Image::mutable_mat_data() {
  // @@protoc_insertion_point(field_mutable:image.Image.mat_data)
  return _internal_mutable_mat_data();
}
inline const std::string& Image::_internal_mat_data() const {
  return mat_data_.GetNoArena();
}
inline void Image::_internal_set_mat_data(const std::string& value) {
  
  mat_data_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void Image::set_mat_data(std::string&& value) {
  
  mat_data_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:image.Image.mat_data)
}
inline void Image::set_mat_data(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  mat_data_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:image.Image.mat_data)
}
inline void Image::set_mat_data(const void* value, size_t size) {
  
  mat_data_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:image.Image.mat_data)
}
inline std::string* Image::_internal_mutable_mat_data() {
  
  return mat_data_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* Image::release_mat_data() {
  // @@protoc_insertion_point(field_release:image.Image.mat_data)
  
  return mat_data_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void Image::set_allocated_mat_data(std::string* mat_data) {
  if (mat_data != nullptr) {
    
  } else {
    
  }
  mat_data_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), mat_data);
  // @@protoc_insertion_point(field_set_allocated:image.Image.mat_data)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace image

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_packet_2eproto
