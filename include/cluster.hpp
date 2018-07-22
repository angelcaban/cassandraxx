//
// Created by eancaba on 6/29/2018.
//

#pragma once

#ifndef CASSANDRACXX_CLUSTER_HPP
#define CASSANDRACXX_CLUSTER_HPP

#include <memory>
#include <string>
#include <cassandra.h>

namespace cassandraxx {

struct CassClusterDeleter {
    void operator() (CassCluster* ptr) {
        cass_cluster_free(ptr);
    }
};

class Cluster {
private:
    friend class Session;

    std::unique_ptr<CassCluster, CassClusterDeleter> cluster_;
    CassCluster* mem() { return cluster_.get(); }

public:
    Cluster() {
        cluster_.reset(cass_cluster_new());
    }
    Cluster(Cluster const& o) = delete;
    ~Cluster() = default;

    CassError setContactPoints(std::string const& contactPoints);
    CassError setPort(int);
    void setCredentials(std::string const& un, std::string const& pw);
    void setSsl(CassSsl*);

    CassError setProtocolVersion(int);
    CassError setBetaProtocol(bool);
    CassError setConsistency(CassConsistency);
    CassError setSerialConsistency(CassConsistency);
    CassError setIOThreads(unsigned);
    CassError setIOQueueSize(unsigned);
    CassError setQueueEventSize(unsigned);
    CassError setCoreConnectionsPerHost(unsigned);
    CassError setMaxConnectionsPerHost(unsigned);
    CassError setMaxConcurrentCreations(unsigned);
    CassError setMaxConcurrentRequestsThreshold(unsigned);
    CassError setMaxRequestsPerFlush(unsigned);
    CassError setPrepareOnAllHosts(bool);
    CassError setNoCompaction(bool);
    void setReconnectWaitTime(unsigned);
    void setConnectTimeout(unsigned);
    void setRequestTimeout(unsigned);
    void setResolveTimeout(unsigned);

    void setTcpNodelay(bool);
    void setTcpKeepalive(bool, unsigned);
    void setUseSchemaMetadata(bool);
    CassError setUseHostnameResolution(bool);

    void setHeartbeatInterval(unsigned);
    void setIdleTimeout(unsigned);

    void setRetryPolicy(CassRetryPolicy*);
    void enableRoundRobinLoadBalancer();
    CassError enableDCAwareLoadBalancer(std::string const& local_dc,
                                        unsigned hosts_per_remote,
                                        bool allow_remote_dc = true);

    void setTokenAwareRouting(bool);

    void setLatencyAwareRouting(bool);
    void configureLatencyAwareRouting(double exclusion_threshold,
                                      uint64_t scale_milliseconds,
                                      uint64_t retry_period_milliseconds,
                                      uint64_t update_rate_milliseconds,
                                      uint64_t min_measured);

    CassError setConstantSpeculativeExecutionPolicy(int64_t delay_milliseconds,
                                                    int max_executions);
    CassError disableSpeculativeExecutionPolicy();

    void setWhitelistFilter(std::string const& hosts);
    void setBlacklistFilter(std::string const& hosts);
    void setWhitelistDcFilter(std::string const& dcs);
    void setBlacklistDcFilter(std::string const& dcs);
    CassError setContactPointRandomization(bool);
};

} // namespace cassandraxx

#endif //CASSANDRACXX_CLUSTER_HPP
