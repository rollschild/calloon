add_executable(calloon)
target_sources(calloon PRIVATE calloon.c)
target_compile_features(calloon PRIVATE c_std_17)
