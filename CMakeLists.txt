add_library(gbaxe.hdll SHARED
  gba.c
)

set_as_hdll(gbaxe)
target_link_libraries(gbaxe.hdll
  libhl
)

install(
  TARGETS gbaxe.hdll
  DESTINATION ${HDLL_DESTINATION}
)
