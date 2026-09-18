if(NOT DEFINED SNAPSHOT_EXECUTABLE OR NOT DEFINED SNAPSHOT_WORKING_DIRECTORY)
    message(FATAL_ERROR "Snapshot cross-process test requires its executable and build directory")
endif()

set(snapshot_media "${SNAPSHOT_WORKING_DIRECTORY}/snapshot-cross-process.img")
set(snapshot_file "${SNAPSHOT_WORKING_DIRECTORY}/snapshot-cross-process.spcs")
set(snapshot_startup_media "${SNAPSHOT_WORKING_DIRECTORY}/snapshot-cross-process-startup.img")
file(REMOVE "${snapshot_media}" "${snapshot_file}" "${snapshot_startup_media}")

execute_process(
    COMMAND "${SNAPSHOT_EXECUTABLE}" save "${snapshot_media}" "${snapshot_file}"
    WORKING_DIRECTORY "${SNAPSHOT_WORKING_DIRECTORY}"
    RESULT_VARIABLE save_result)
if(NOT save_result EQUAL 0)
    file(REMOVE "${snapshot_media}" "${snapshot_file}" "${snapshot_startup_media}")
    message(FATAL_ERROR "Snapshot writer process failed: ${save_result}")
endif()

execute_process(
    COMMAND "${SNAPSHOT_EXECUTABLE}" load-mismatch "${snapshot_startup_media}" "${snapshot_file}"
    WORKING_DIRECTORY "${SNAPSHOT_WORKING_DIRECTORY}"
    RESULT_VARIABLE mismatch_result)
if(NOT mismatch_result EQUAL 0)
    file(REMOVE "${snapshot_media}" "${snapshot_file}" "${snapshot_startup_media}")
    message(FATAL_ERROR "Snapshot mismatch reader process failed: ${mismatch_result}")
endif()

execute_process(
    COMMAND "${SNAPSHOT_EXECUTABLE}" load "${snapshot_startup_media}" "${snapshot_file}"
    WORKING_DIRECTORY "${SNAPSHOT_WORKING_DIRECTORY}"
    RESULT_VARIABLE load_result)
if(NOT load_result EQUAL 0)
    file(REMOVE "${snapshot_media}" "${snapshot_file}" "${snapshot_startup_media}")
    message(FATAL_ERROR "Snapshot reader process failed: ${load_result}")
endif()

file(REMOVE "${snapshot_media}" "${snapshot_file}" "${snapshot_startup_media}")
