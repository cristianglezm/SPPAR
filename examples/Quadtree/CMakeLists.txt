
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/Quadtree)

# all source files
set(Quadtree_SRC ${SRCROOT}/Quadtree.cpp)

# define the XOR target
add_executable(Quadtree ${Quadtree_SRC})

install(TARGETS Quadtree RUNTIME DESTINATION examples ARCHIVE DESTINATION examples)