#!/bin/bash
set -e

FILTER_RULE=${FILTER_RULE:-port 80 or port 443}
FINGERPRINT_DATABASE=${FINGERPRINT_DATABASE:-/opt/p0f/etc/p0f.fp}
PARALLEL_API_CONNECTIONS=${PARALLEL_API_CONNECTIONS:-100}
CONNECTION_HOST_CACHE_LIMIT=${CONNECTION_HOST_CACHE_LIMIT:-30,120}
CONNECTION_HOST_CAP_LIMIT=${CONNECTION_HOST_CAP_LIMIT:-1000,10000}

if [ "$1" == "p0f" ]; then
	POF_OPTS=" -f ${FINGERPRINT_DATABASE}"
	POF_OPTS+=" -t ${CONNECTION_HOST_CACHE_LIMIT}"
	POF_OPTS+=" -m ${CONNECTION_HOST_CAP_LIMIT}"

	if [ ! -z "${INTERFACE}" ]; then
		POF_OPTS+=" -i ${INTERFACE}"
	fi

	if [ ! -z "${UNIX_SOCKET}" ]; then
		POF_OPTS+=" -s ${UNIX_SOCKET}"
		POF_OPTS+=" -S ${PARALLEL_API_CONNECTIONS}"
	fi

	if [ ! -z "${LOG_FILE}" ]; then
		POF_OPTS+=" -o ${LOG_FILE}"
	fi

	exec /opt/p0f/bin/p0f $POF_OPTS "${FILTER_RULE}" 
fi

exec "$@"
