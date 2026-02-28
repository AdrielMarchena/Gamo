# Vendor Instructions

## Role
Vendor contains third-party libraries as git submodules.

Location:
vendor/library_name/

## Rules
- Never modify vendor code
- Never reformat vendor code
- Never apply clang-format to vendor
- Never apply clang-tidy to vendor

## Integration
Use add_subdirectory(vendor/library)

Link using exported targets.

Example:
target_link_libraries(engine PRIVATE glfw)