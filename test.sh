#!/bin/sh

# Simple cmd
TEST_SIMPLE_CMD1="echo \"This is a test!\""
TEST_SIMPLE_CMD2="ls"
# Redirections out
TEST_SIMPLE_CMD_OUT1="env > test_simple_cmd_out1"
TEST_SIMPLE_CMD_OUT2="set > test_simple_cmd_out2"
TEST_SIMPLE_CMD_OUT_v2_1="env > test_simple_cmd_out_v2_empty1 > test_simple_cmd_out_v2_1"
TEST_SIMPLE_CMD_OUT_v2_2="set > test_simple_cmd_out_v2_empty2 > test_simple_cmd_out_v2_2"
TEST_SIMPLE_CMD_OUT_v3_1="env > test_simple_cmd_out_v3_empty1_1 > test_simple_cmd_out_v3_empty2_1 >test_simple_cmd_out_v3_1"
TEST_SIMPLE_CMD_OUT_v3_2="set > test_simple_cmd_out_v3_empty1_2 > test_simple_cmd_out_v3_empty2_2 >test_simple_cmd_out_v3_2"
# Semicolons
TEST_SEMICOLON1="echo \"Semicolon Test 1.1\"; echo \"Test 1.2\""
TEST_SEMICOLON2="echo \"Semicolon Test 2.1\";"
TEST_SEMICOLON3="echo \"Semicolon Test 3.1\" ;"
# Redirection in
TEST_SIMPLE_CMD_IN="echo \"Test 1 simple redirection stdin\" > in1 ; cat -e < in1"
TEST_SIMPLE_CMD_IN_v2="echo \"Test 2 simple redirection stdin\" > in2 ; cat -e < in2_empty1 < in2"
TEST_SIMPLE_CMD_IN_v3="echo \"Test 3 simple redirection stdin\" > in3 ; cat -e < in3_empty1 < in3_empty2 < in3"
# Pipes
# to do




#Utils
TEST_DIR="tests"
TEST_LOG="log"

#All tests
TESTS=(${TEST_SIMPLE_CMD1} ${TEST_SIMPLE_CMD2} ${TEST_SIMPLE_CMD_OUT1} ${TEST_SIMPLE_CMD_OUT2} \
${TEST_SIMPLE_CMD_OUT_v2_1} ${TEST_SIMPLE_CMD_OUT_v2_2} ${TEST_SIMPLE_CMD_OUT_v3_1} ${TEST_SIMPLE_CMD_OUT_v3_2} \
${TEST_SEMICOLON1} ${TEST_SEMICOLON2})

if [ ! -f "minshell" ]; then
    mkdir -p "${TEST_DIR}"
    for TEST in TESTS[@]; do
        echo ${TEST} >> "${TEST_DIR}/${TEST_LOG}"
    done
    cp ./minshell "${TEST_DIR}"
    cd "${TEST_DIR}"
    ./minshell < log 2 > /dev/null
fi