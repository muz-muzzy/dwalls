find_path(HEIF_INCLUDE_DIR NAMES libheif/heif.h)
find_library(HEIF_LIBRARY NAMES heif)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(heif DEFAULT_MSG HEIF_LIBRARY HEIF_INCLUDE_DIR)

if(HEIF_FOUND AND NOT TARGET heif)
  add_library(heif UNKNOWN IMPORTED)
  set_target_properties(heif PROPERTIES
    IMPORTED_LOCATION "${HEIF_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${HEIF_INCLUDE_DIR}")
endif()

mark_as_advanced(HEIF_INCLUDE_DIR HEIF_LIBRARY)