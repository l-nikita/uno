#!/bin/bash
# =============================================================================
# test_online.sh — Launch multiple Uno instances for network testing
#
# Usage:
#   ./test_online.sh                 Start host + 1 client (local)
#   ./test_online.sh --host          Host only
#   ./test_online.sh --connect <ip>  Client only, connect to <ip>
#   ./test_online.sh --clients <N>   Host + N clients (default: 1)
# =============================================================================

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
UNO_BIN="$SCRIPT_DIR/build/uno"

PORT=27015
MODE=""
CONNECT_IP=""
NUM_CLIENTS=1
PIDS=()

while [[ $# -gt 0 ]]; do
    case "$1" in
        --host)
            MODE="host"
            shift
            ;;
        --connect)
            MODE="connect"
            CONNECT_IP="${2:?'--connect requires an IP address'}"
            shift 2
            ;;
        --port)
            PORT="${2:?'--port requires a port number'}"
            shift 2
            ;;
        --clients)
            NUM_CLIENTS="${2:?'--clients requires a number'}"
            shift 2
            ;;
        --bin)
            UNO_BIN="${2:?'--bin requires a path to the uno binary'}"
            shift 2
            ;;
        -h|--help)
            echo "Usage:"
            echo "  $0                      Host + 1 client (local test)"
            echo "  $0 --host               Start only the host"
            echo "  $0 --connect <ip>       Connect to existing host"
            echo "  $0 --clients <N>        Host + N clients (default: 1)"
            echo "  $0 --port <port>        Server port (default: 27015)"
            echo "  $0 --bin <path>         Path to uno binary"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

if [[ ! -x "$UNO_BIN" ]]; then
    echo "Error: '$UNO_BIN' not found or not executable."
    echo "Build the project first, or specify --bin <path>."
    exit 1
fi


cleanup() {
    echo ""
    echo "Stopping all instances..."
    for pid in "${PIDS[@]}"; do
        if kill -0 "$pid" 2>/dev/null; then
            kill "$pid" 2>/dev/null || true
        fi
    done
    wait 2>/dev/null
    echo "Done."
}
trap cleanup EXIT INT TERM

cd "$SCRIPT_DIR/build"

if [[ "$MODE" == "connect" ]]; then
    echo "Connecting to $CONNECT_IP:$PORT..."
    "$UNO_BIN" --connect "$CONNECT_IP" --port "$PORT" &
    PIDS+=($!)

elif [[ "$MODE" == "host" ]]; then
    echo "Starting host on port $PORT..."
    "$UNO_BIN" --host --port "$PORT" --name "Host" &
    PIDS+=($!)

else
    echo "Starting host on port $PORT..."
    "$UNO_BIN" --host --port "$PORT" --name "Host" &
    PIDS+=($!)

    sleep 1

    for i in $(seq 1 "$NUM_CLIENTS"); do
        echo "Starting client #$i..."
        "$UNO_BIN" --connect "127.0.0.1" --port "$PORT" --name "Player_$i" &
        PIDS+=($!)
        sleep 0.5
    done
fi

echo ""
echo "=== Running ${#PIDS[@]} instance(s). Press Ctrl+C to stop all. ==="
echo ""

wait
