set_property(GLOBAL PROPERTY USE_FOLDERS On)
PROJECT( server )

set( SERVER_INCLUDE_PATH
	${INCLUDE_PATH}
  	${root_src_dir}/server/src
)
	
link_directories( ${DEP_LIB_PATH} )
include_directories( ${SERVER_INCLUDE_PATH} )

aux_source_directory( ${root_src_dir}/server/src		SERVER_SRC )
aux_source_directory( ${root_src_dir}/share/proto-cpp	PROTO_SRC )
aux_source_directory( ${root_src_dir}/event-net/src		NET_SRC )
aux_source_directory( ${root_src_dir}/io-event/src		IO_SRC )

add_executable( ${PROJECT_NAME} ${SERVER_SRC} ${PROTO_SRC} ${NET_SRC} ${IO_SRC} )

set(
    SHARED_LIBS
    ${SHARED_LIBS}
)

TARGET_LINK_LIBRARIES( ${PROJECT_NAME}  ${SHARED_LIBS} )

INSTALL( TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${root_src_dir}/public/server
    )
