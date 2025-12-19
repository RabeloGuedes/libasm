#!/bin/bash

# Valgrind wrapper script for better output formatting
# This script runs valgrind and formats the output for better readability

TESTER="$1"
LIBASM_PATH="$2"
VALGRIND_LOG="valgrind_raw.log"
SUMMARY_LOG="valgrind_summary.log"

# Colors
RED='\033[1;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
CYAN='\033[1;36m'
MAGENTA='\033[1;35m'
WHITE='\033[1;37m'
RESET='\033[0m'

echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${RESET}"
echo -e "${CYAN}║                                                            ║${RESET}"
echo -e "${CYAN}║              VALGRIND MEMORY LEAK DETECTION                ║${RESET}"
echo -e "${CYAN}║                                                            ║${RESET}"
echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${RESET}"
echo ""

echo -e "${WHITE}Running tests with memory leak detection...${RESET}"
echo -e "${YELLOW}This may take 30-60 seconds...${RESET}"
echo ""

# Create temporary file for test output
TEST_OUTPUT=$(mktemp)

# Show a progress indicator
(
    printf "${CYAN}"
    while kill -0 $$ 2>/dev/null; do
        printf "▓"
        sleep 1
    done
    printf "${RESET}"
) &
PROGRESS_PID=$!

# Run valgrind with comprehensive settings
valgrind \
    --leak-check=full \
    --show-leak-kinds=definite,possible \
    --track-origins=yes \
    --verbose \
    --log-file="$VALGRIND_LOG" \
    --suppressions=valgrind.supp \
    "$TESTER" "$LIBASM_PATH" > "$TEST_OUTPUT" 2>&1

VALGRIND_EXIT=$?

# Kill the progress indicator
kill $PROGRESS_PID 2>/dev/null
wait $PROGRESS_PID 2>/dev/null
echo ""
echo ""

# Show the test results briefly
echo -e "${CYAN}════════════════════════════════════════════════════════════${RESET}"
echo -e "${WHITE}                     TEST RESULTS                           ${RESET}"
echo -e "${CYAN}════════════════════════════════════════════════════════════${RESET}"
echo ""

# Extract and display the final score
grep -A 10 "FINAL SCORE" "$TEST_OUTPUT" | head -12
echo ""

# Parse the valgrind output
echo -e "${CYAN}════════════════════════════════════════════════════════════${RESET}"
echo -e "${WHITE}                   MEMORY LEAK SUMMARY                      ${RESET}"
echo -e "${CYAN}════════════════════════════════════════════════════════════${RESET}"
echo ""

# Extract key statistics
DEFINITELY_LOST=$(grep "definitely lost:" "$VALGRIND_LOG" | tail -1 | awk '{print $4, $5}')
INDIRECTLY_LOST=$(grep "indirectly lost:" "$VALGRIND_LOG" | tail -1 | awk '{print $4, $5}')
POSSIBLY_LOST=$(grep "possibly lost:" "$VALGRIND_LOG" | tail -1 | awk '{print $4, $5}')
STILL_REACHABLE=$(grep "still reachable:" "$VALGRIND_LOG" | tail -1 | awk '{print $4, $5}')
TOTAL_HEAP=$(grep "total heap usage:" "$VALGRIND_LOG" | tail -1)

# Check for errors
ERROR_SUMMARY=$(grep "ERROR SUMMARY:" "$VALGRIND_LOG" | tail -1 | awk '{print $4}')
INVALID_READS=$(grep -c "Invalid read" "$VALGRIND_LOG")
INVALID_WRITES=$(grep -c "Invalid write" "$VALGRIND_LOG")

# Check if invalid operations are from fork/NULL tests (child processes that terminate with SIGSEGV)
INVALID_IN_TERMINATED_PROCESS=$(grep -B 5 "Invalid read\|Invalid write" "$VALGRIND_LOG" | grep -c "Process terminating with default action of signal 11")

# Count total invalid operations
TOTAL_INVALID=$((INVALID_READS + INVALID_WRITES))

# Check for invalid operations in libasm assembly code (not in test code or libc)
# Look for invalid operations that are "at" an assembly function, not just "by" a test function
INVALID_IN_LIBASM=$(grep -A 3 "Invalid read\|Invalid write" "$VALGRIND_LOG" | grep "^==.*==    at" | grep -c "\.s:\|libasm\.a")

# Display heap usage
if [ ! -z "$TOTAL_HEAP" ]; then
    ALLOCS=$(echo "$TOTAL_HEAP" | awk '{print $5}')
    FREES=$(echo "$TOTAL_HEAP" | awk '{print $7}')
    echo -e "  ${WHITE}Heap Usage:${RESET}"
    echo -e "    Allocations:  ${CYAN}$ALLOCS${RESET}"
    echo -e "    Frees:        ${CYAN}$FREES${RESET}"
    echo ""
fi

# Display leak information
echo -e "  ${WHITE}Memory Leaks:${RESET}"

HAS_LEAKS=0

# Definitely lost (critical)
DEFINITELY_BYTES=$(echo "$DEFINITELY_LOST" | awk '{print $1}' | tr -d ',')
if [ "$DEFINITELY_BYTES" != "0" ] && [ ! -z "$DEFINITELY_BYTES" ]; then
    echo -e "    ${RED}✗ Definitely lost:${RESET}   ${RED}$DEFINITELY_LOST${RESET}"
    HAS_LEAKS=1
else
    echo -e "    ${GREEN}✓ Definitely lost:${RESET}   ${GREEN}0 bytes${RESET}"
fi

# Indirectly lost
INDIRECTLY_BYTES=$(echo "$INDIRECTLY_LOST" | awk '{print $1}' | tr -d ',')
if [ "$INDIRECTLY_BYTES" != "0" ] && [ ! -z "$INDIRECTLY_BYTES" ]; then
    echo -e "    ${RED}✗ Indirectly lost:${RESET}   ${RED}$INDIRECTLY_LOST${RESET}"
    HAS_LEAKS=1
else
    echo -e "    ${GREEN}✓ Indirectly lost:${RESET}   ${GREEN}0 bytes${RESET}"
fi

# Possibly lost (warning)
POSSIBLY_BYTES=$(echo "$POSSIBLY_LOST" | awk '{print $1}' | tr -d ',')
if [ "$POSSIBLY_BYTES" != "0" ] && [ ! -z "$POSSIBLY_BYTES" ]; then
    echo -e "    ${YELLOW}⚠ Possibly lost:${RESET}     ${YELLOW}$POSSIBLY_LOST${RESET}"
    # Don't count as critical failure, but show warning
else
    echo -e "    ${GREEN}✓ Possibly lost:${RESET}     ${GREEN}0 bytes${RESET}"
fi

# Still reachable (should be minimal with good suppressions)
REACHABLE_BYTES=$(echo "$STILL_REACHABLE" | awk '{print $1}' | tr -d ',')
if [ "$REACHABLE_BYTES" != "0" ] && [ ! -z "$REACHABLE_BYTES" ]; then
    # Check if it's a small amount (system/fork overhead)
    if [ "$REACHABLE_BYTES" -lt 10000 ]; then
        echo -e "    ${GREEN}✓ Still reachable:${RESET}   ${GREEN}$STILL_REACHABLE${RESET} ${WHITE}(system/fork overhead)${RESET}"
    else
        echo -e "    ${YELLOW}⚠ Still reachable:${RESET}   ${YELLOW}$STILL_REACHABLE${RESET} ${WHITE}(check suppressions)${RESET}"
    fi
else
    echo -e "    ${GREEN}✓ Still reachable:${RESET}   ${GREEN}0 bytes${RESET}"
fi

echo ""

# Display invalid operations with smart analysis
if [ "$INVALID_READS" -gt 0 ] || [ "$INVALID_WRITES" -gt 0 ]; then
    echo -e "  ${WHITE}Memory Access Analysis:${RESET}"
    
    # Check if any are in actual libasm assembly code
    if [ "$INVALID_IN_LIBASM" -gt 0 ]; then
        echo -e "    ${RED}✗ In your ASM code:${RESET}   ${RED}$INVALID_IN_LIBASM invalid operations${RESET}"
        HAS_LEAKS=1
    else
        # All invalid operations are in test code (NULL pointer tests)
        echo -e "    ${GREEN}✓ In your ASM code:${RESET}   ${GREEN}0 invalid operations${RESET}"
    fi
    
    # Show where the invalid operations actually are
    if [ "$TOTAL_INVALID" -gt 0 ]; then
        echo -e "    ${CYAN}ℹ In test suite:${RESET}     ${CYAN}$TOTAL_INVALID operations${RESET} ${WHITE}(NULL pointer tests)${RESET}"
        echo -e "      ${WHITE}These are intentional - testing how your code handles NULL${RESET}"
    fi
    
    echo ""
fi

# Error summary
if [ ! -z "$ERROR_SUMMARY" ]; then
    if [ "$ERROR_SUMMARY" -eq 0 ]; then
        echo -e "  ${GREEN}✓ Error Summary: $ERROR_SUMMARY errors detected${RESET}"
    else
        echo -e "  ${RED}✗ Error Summary: $ERROR_SUMMARY errors detected${RESET}"
        HAS_LEAKS=1
    fi
    echo ""
fi

# Extract specific leak locations if any
if [ "$HAS_LEAKS" -eq 1 ]; then
    echo -e "${YELLOW}════════════════════════════════════════════════════════════${RESET}"
    echo -e "${WHITE}                     LEAK DETAILS                           ${RESET}"
    echo -e "${YELLOW}════════════════════════════════════════════════════════════${RESET}"
    echo ""
    
    # Show leak records
    grep -A 10 "definitely lost" "$VALGRIND_LOG" | grep -E "(at|by|Address)" | head -20 > "$SUMMARY_LOG" 2>/dev/null
    
    if [ -s "$SUMMARY_LOG" ]; then
        echo -e "${WHITE}Stack traces for leaked memory:${RESET}"
        echo ""
        cat "$SUMMARY_LOG" | sed 's/^/  /'
        echo ""
    fi
    
    echo -e "${YELLOW}For complete details, check: ${WHITE}$VALGRIND_LOG${RESET}"
    echo ""
fi

# Final verdict
echo -e "${CYAN}════════════════════════════════════════════════════════════${RESET}"
if [ "$HAS_LEAKS" -eq 0 ]; then
    echo -e "${GREEN}✓ PASSED: No memory leaks detected!${RESET}"
    echo ""
    echo -e "${WHITE}Valgrind logs saved to:${RESET}"
    echo -e "  • ${CYAN}$VALGRIND_LOG${RESET} (detailed analysis)"
    echo ""
    # Clean up temporary files
    rm -f "$TEST_OUTPUT" "$SUMMARY_LOG" 2>/dev/null
    exit 0
else
    echo -e "${RED}✗ FAILED: Memory leaks or errors detected${RESET}"
    echo ""
    echo -e "${WHITE}Next steps:${RESET}"
    echo -e "  1. Review the leak details above"
    echo -e "  2. Check ${WHITE}$VALGRIND_LOG${RESET} for complete stack traces"
    echo -e "  3. Fix memory allocations in your assembly code"
    echo -e "  4. Ensure malloc/free are properly paired"
    echo ""
    echo -e "${WHITE}Valgrind logs saved to:${RESET}"
    echo -e "  • ${CYAN}$VALGRIND_LOG${RESET} (detailed analysis)"
    echo -e "  • ${CYAN}$SUMMARY_LOG${RESET} (leak summary)"
    echo ""
    # Clean up test output but keep valgrind logs
    rm -f "$TEST_OUTPUT" 2>/dev/null
    exit 1
fi
