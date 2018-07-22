//
// Created by eancaba on 6/29/2018.
//

#include <cluster.hpp>

namespace cassandraxx {

void Cluster::configureLatencyAwareRouting(double exclusion_threshold,
                                           uint64_t scale_milliseconds,
                                           uint64_t retry_period_milliseconds,
                                           uint64_t update_rate_milliseconds,
                                           uint64_t min_measured) {
    cass_cluster_set_latency_aware_routing_settings(mem(),
                                                    exclusion_threshold,
                                                    scale_milliseconds,
                                                    retry_period_milliseconds,
                                                    update_rate_milliseconds,
                                                    min_measured);
}

CassError Cluster::disableSpeculativeExecutionPolicy() {
    return cass_cluster_set_no_speculative_execution_policy(mem());
}

CassError Cluster::enableDCAwareLoadBalancer(std::string const &local_dc,
                                             unsigned hosts_per_remote,
                                             bool allow_remote_dc) {
    return cass_cluster_set_load_balance_dc_aware_n(mem(),
                                                    local_dc.c_str(), local_dc.length(),
                                                    hosts_per_remote,
                                                    static_cast<cass_bool_t>(allow_remote_dc));
}

void Cluster::enableRoundRobinLoadBalancer() {
    cass_cluster_set_load_balance_round_robin(mem());
}

void Cluster::setTokenAwareRouting(bool enable) {
    cass_cluster_set_token_aware_routing(mem(),
                                         static_cast<cass_bool_t>(enable));
}

void Cluster::setLatencyAwareRouting(bool enable) {
    cass_cluster_set_latency_aware_routing(mem(),
                                           static_cast<cass_bool_t>(enable));
}

CassError Cluster::setBetaProtocol(bool enable) {
    return cass_cluster_set_use_beta_protocol_version(mem(),
                                                      static_cast<cass_bool_t>(enable));
}

void Cluster::setBlacklistDcFilter(std::string const &dcs) {
    cass_cluster_set_blacklist_dc_filtering_n(mem(),
                                              dcs.c_str(), dcs.length());
}

void Cluster::setBlacklistFilter(std::string const &hosts) {
    cass_cluster_set_blacklist_filtering_n(mem(),
                                           hosts.c_str(), hosts.length());
}

void Cluster::setWhitelistDcFilter(std::string const &dcs) {
    cass_cluster_set_whitelist_dc_filtering_n(mem(),
                                              dcs.c_str(), dcs.length());
}

void Cluster::setWhitelistFilter(std::string const &hosts) {
    cass_cluster_set_whitelist_filtering_n(mem(),
                                           hosts.c_str(), hosts.length());
}

void Cluster::setConnectTimeout(unsigned val) {
    cass_cluster_set_connect_timeout(mem(), val);
}

CassError Cluster::setConsistency(CassConsistency consistency) {
    return cass_cluster_set_consistency(mem(),
                                        consistency);
}

CassError Cluster::setConstantSpeculativeExecutionPolicy(int64_t delay_ms,
                                                         int max_executions) {
    return cass_cluster_set_constant_speculative_execution_policy(
            mem(),
            delay_ms,
            max_executions);
}

CassError Cluster::setContactPointRandomization(bool enable) {
    return cass_cluster_set_use_randomized_contact_points(
            mem(),
            static_cast<cass_bool_t>(enable));
}

CassError Cluster::setContactPoints(std::string const &contactPoints) {
    return cass_cluster_set_contact_points_n(mem(),
                                             contactPoints.c_str(),
                                             contactPoints.length());

}

CassError Cluster::setPort(int port) {
    return cass_cluster_set_port(mem(), port);
}

CassError Cluster::setCoreConnectionsPerHost(unsigned num) {
    return cass_cluster_set_core_connections_per_host(mem(),
                                                      num);
}

void Cluster::setCredentials(std::string const &un,
                             std::string const &pw) {
    cass_cluster_set_credentials_n(mem(),
                                   un.c_str(), un.length(),
                                   pw.c_str(), pw.length());
}

void Cluster::setSsl(CassSsl *ssl) {
    cass_cluster_set_ssl(mem(), ssl);
}

CassError Cluster::setProtocolVersion(int version) {
    return cass_cluster_set_protocol_version(mem(), version);
}

CassError Cluster::setSerialConsistency(CassConsistency consistency) {
    return cass_cluster_set_serial_consistency(mem(),
                                               consistency);
}

CassError Cluster::setIOQueueSize(unsigned size) {
    return cass_cluster_set_queue_size_io(mem(), size);
}

CassError Cluster::setQueueEventSize(unsigned size) {
    return cass_cluster_set_queue_size_event(mem(), size);
}

CassError Cluster::setIOThreads(unsigned num) {
    return cass_cluster_set_num_threads_io(mem(), num);
}

CassError Cluster::setMaxConcurrentCreations(unsigned num) {
    return cass_cluster_set_max_concurrent_creation(mem(),
                                                    num);
}

CassError Cluster::setMaxConnectionsPerHost(unsigned num) {
    return cass_cluster_set_max_connections_per_host(mem(),
                                                     num);
}

CassError Cluster::setMaxConcurrentRequestsThreshold(unsigned num) {
    return cass_cluster_set_max_concurrent_requests_threshold(mem(),
                                                              num);
}

CassError Cluster::setMaxRequestsPerFlush(unsigned num) {
    return cass_cluster_set_max_requests_per_flush(mem(), num);
}

CassError Cluster::setPrepareOnAllHosts(bool val) {
    return cass_cluster_set_prepare_on_all_hosts(mem(),
                                                 static_cast<cass_bool_t>(val));
}

CassError Cluster::setNoCompaction(bool val) {
    return cass_cluster_set_no_compact(mem(), static_cast<cass_bool_t>(val));
}

void Cluster::setReconnectWaitTime(unsigned time) {
    cass_cluster_set_reconnect_wait_time(mem(), time);
}

void Cluster::setRequestTimeout(unsigned val) {
    cass_cluster_set_request_timeout(mem(), val);
}

void Cluster::setResolveTimeout(unsigned val) {
    cass_cluster_set_resolve_timeout(mem(), val);
}

void Cluster::setTcpKeepalive(bool enable, unsigned time) {
    cass_cluster_set_tcp_keepalive(mem(),
                                   static_cast<cass_bool_t>(enable),
                                   time);
}

void Cluster::setTcpNodelay(bool enable) {
    cass_cluster_set_tcp_nodelay(mem(),
                                 static_cast<cass_bool_t>(enable));
}

CassError Cluster::setUseHostnameResolution(bool enable) {
    return cass_cluster_set_use_hostname_resolution(mem(),
                                                    static_cast<cass_bool_t>(enable));
}

void Cluster::setUseSchemaMetadata(bool enable) {
    cass_cluster_set_use_schema(mem(), static_cast<cass_bool_t>(enable));
}

void Cluster::setHeartbeatInterval(unsigned interval) {
    cass_cluster_set_connection_heartbeat_interval(mem(), interval);
}

void Cluster::setIdleTimeout(unsigned timeout) {
    cass_cluster_set_connection_idle_timeout(mem(), timeout);
}

void Cluster::setRetryPolicy(CassRetryPolicy *policy) {
    cass_cluster_set_retry_policy(mem(), policy);
}


} // namespace cassandraxx
