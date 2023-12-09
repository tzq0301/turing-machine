set -e

bash build.sh >> /dev/null
./bin/turing "$@"