#!/bin/bash

C_RED="\033[31m"
C_GRN="\033[32m"
C_YLW="\033[33m"
C_BLU="\033[34m"
C_CYN="\033[36m"
C_ULN="\033[4m"
C_CLR="\033[0m"

DIR_RUN="."
DIR_TESTS="$(pwd)/tests"

MAKE_FILE="false"
SINGLE_FILE_NAME="src/main.cpp"

EXE="main.out"
EXE_OUTPUT="out.txt"
COMPILER="g++ -std=c++20"

TIME_LIMIT=2s
DIFF_TOOL="sdiff -sWB"
VERBOSE_DIFF_TOOL="sdiff -Wsw 108"

function remove_colors() {
    sed -r "s/\x1B\[[0-9;]*[mK]//g"
}

function cxx_compile {
    local file_name=$SINGLE_FILE_NAME
    echo -e "${C_CYN}${C_ULN}Compiling $file_name...${C_CLR}"
    if [[ ! -e "$file_name" ]]; then
        echo -e "${C_RED}File $file_name does not exist${C_CLR}" >&2
        return 1
    fi
    if ! $COMPILER "$file_name" -o $EXE >/dev/null 2>&1; then
        echo -e "${C_RED}Compile Error${C_CLR}" >&2
        return 1
    fi
    echo -e "Compiled Successfully"
    return 0
}

function compile_and_run {
    local verbose="false"
    [[ $3 == "true" ]] && verbose="true"

    local file_name=$1
    local test_dir=$2
    local passed=0
    local failed=0

    cxx_compile "$file_name" || return

    if [[ ! -f "$EXE" ]]; then
        echo -e "${C_RED}EXE not found${C_CLR}" >&2
        return 1
    fi

    echo -e "Running tests [DIR: $test_dir]"

    for testcase in "$test_dir"/*; do
        [[ -d "$testcase" ]] || continue

        [[ "$testcase" == "$test_dir/csv" ]] && continue

        local number=$(basename "$testcase")
        local input="$testcase/$number.in"
        local solution="$testcase/$number.out"

        echo -e "${C_YLW}Running test: $number${C_CLR}"

        if ! timeout $TIME_LIMIT ./$EXE < "$input" >"$EXE_OUTPUT" 2>&1; then
            echo -e "${C_RED}Timed out${C_CLR}"
            ((failed++))
            continue
        fi

        if $DIFF_TOOL "$EXE_OUTPUT" "$solution" >/dev/null; then
            echo -e "${C_GRN}Accepted${C_CLR}"
            ((passed++))
            rm -f "$testcase/$number.diff" "$testcase/$number.result" 2>/dev/null
        else
            echo -e "${C_RED}Wrong answer${C_CLR}"
            ((failed++))
            if [[ $verbose == "true" ]]; then
                printf "%52s | %52s\n" "< $EXE_OUTPUT" "> solution"
                $VERBOSE_DIFF_TOOL "$EXE_OUTPUT" "$solution"
            fi
            diff "$EXE_OUTPUT" "$solution" >"$testcase/$number.diff"
            cat "$EXE_OUTPUT" >"$testcase/$number.result"
        fi
    done

    echo
    echo -e "        Passed: ${C_GRN}$passed${C_CLR} out of $((passed + failed))"
    echo -e "        Failed: ${C_RED}$failed${C_CLR} out of $((passed + failed))"
    echo

    rm -f "$EXE_OUTPUT"
}

function run_single_file_tests {
    local verbose="false"
    [[ $1 == "true" ]] && verbose="true"

    if [[ $(find . -maxdepth 1 -type f | wc -l) -ge 1 ]]; then
        local file_name=$(find . -maxdepth 1 -type f -name "*.cpp" -print -quit)
        echo -e "${C_YLW}Found single file: $file_name${C_CLR}"
        compile_and_run "$file_name" "$DIR_TESTS" "$verbose"
        return
    fi
    echo -e "${C_YLW}No file found, or found multiple candidates.${C_CLR}"
    compile_and_run "$SINGLE_FILE_NAME" "$DIR_TESTS" "$verbose"
}

function test_code() {
    echo -e "${C_BLU}${C_ULN}Testing...${C_CLR} [DIR: $DIR_RUN]"
    if [[ ! -d $DIR_RUN ]]; then
        echo -e "${C_RED}Directory not found.${C_CLR} [${DIR_RUN}]" >&2
        exit 1
    fi
    pushd "$DIR_RUN" >/dev/null
    run_single_file_tests "$verbose"
    popd >/dev/null
}

function remove_test_diff() {
    pushd "$DIR_TESTS" >/dev/null
    find . -type f -name "*.result" -delete
    find . -type f -name "*.diff" -delete
    popd >/dev/null
}

while [[ $# -gt 0 ]]; do
    case $1 in
    -t | --test)
        test="true"
        if [[ -n $2 && ! $2 =~ ^-.* ]]; then
            DIR_RUN=$2
            shift
        fi
        shift
        ;;
    -r | --remove-test-diff)
        remove_test_diff="true"
        shift
        ;;
    -*)
        echo "Unknown option: $1" >&2
        exit 1
        ;;
    *)
        echo "Script takes no positional arguments: $1" >&2
        exit 1
        ;;
    esac
done

: ${test:="false"}
: ${remove_test_diff:="false"}

if [[ ! -e "$DIR_RUN/Makefile" ]]; then
    MAKE_FILE="false"
fi

if [[ $test == "true" ]]; then
    test_code
fi

if [[ $remove_test_diff == "true" ]]; then
    echo -e "${C_BLU}${C_ULN}Removing test diff...${C_CLR}"
    if ! remove_test_diff; then
        echo -e "${C_RED}Error while removing.${C_CLR}" >&2
        exit 1
    fi
    echo -e "${C_GRN}Test diff removed.${C_CLR}"
fi
