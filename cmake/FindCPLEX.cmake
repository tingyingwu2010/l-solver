# All credits go to https://github.com/coin-or/Gravity/blob/master/CMakeLists.txt

file(GLOB dirs /opt/ibm/ILOG/CPLEX_Studio_Community*)
foreach(d in ${dirs})
    string(REGEX MATCH "[0-9]+" CPLEX_VERSION "${d}")
endforeach(d)

message("Cplex version ${CPLEX_VERSION}")
string(CONCAT CPLEX_DIR /opt/ibm/ILOG/CPLEX_Studio_Community;${CPLEX_VERSION};/cplex)
string(CONCAT CONCERT_DIR /opt/ibm/ILOG/CPLEX_Studio_Community;${CPLEX_VERSION};/concert)
message("Looking for Cplex in ${CPLEX_DIR}")

find_path(CPLEX_INCLUDE_DIR ilocplex.h HINTS "${CPLEX_DIR}/include/ilcplex")
find_path(CONCERT_INCLUDE_DIR iloalg.h HINTS "${CONCERT_DIR}/include/ilconcert")
#find_path(CPLEX_LINK libcplex.a x HINTS "${CPLEX_DIR}/lib/x86-64_linux/static_pic")
#find_path(CPLEX_LINK2 libconcert.a  HINTS "${CONCERT_DIR}/lib/x86-64_linux/static_pic")
message("Found for Cplex Link directory")
find_library(CPLEX_CPP_LIBRARY libcplex.a x HINTS "${CPLEX_DIR}/lib/x86-64_linux/static_pic")
find_library(CONCERT_CPP_LIBRARY libconcert.a libcplexdistmip.a HINTS "${CONCERT_DIR}/lib/x86-64_linux/static_pic")
find_library(CPLEX_ILO_CPP_LIBRARY libilocplex.a HINTS "${CPLEX_DIR}/lib/x86-64_linux/static_pic")
set(LINUX_CXX_FLAGS "-Wno-ignored-attributes -ldl")
#find_library(CPLEX_DIST_CPP_LIBRARY libcplexdistmip.a HINTS "${CPLEX_DIR}/lib/x86-64_linux/static_pic")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CPLEX DEFAULT_MSG CPLEX_CPP_LIBRARY  CPLEX_ILO_CPP_LIBRARY CONCERT_CPP_LIBRARY  CPLEX_INCLUDE_DIR CONCERT_INCLUDE_DIR)

if(CPLEX_FOUND)
    set(CPX_LICENSE_FILE "~/cplex.research.lic")
    set(CPLEX_INCLUDE_DIRS ${CPLEX_INCLUDE_DIR}/.. ${CONCERT_INCLUDE_DIR}/..)
    set(CPLEX_LIBRARIES ${CPLEX_ILO_CPP_LIBRARY} ${CPLEX_CPP_LIBRARY}  ${CONCERT_CPP_LIBRARY})
endif(CPLEX_FOUND)

mark_as_advanced(CPLEX_LIBRARY CPLEX_CPP_LIBRARY CPLEX_INCLUDE_DIR)