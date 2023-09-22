#include "database.hpp"

namespace Minor {
pqxx::result AccountingDatabase::smartQuery(pqxx::work &query, const std::string &request) const {
    if (showRequestLogs) LOG("QUERY", request);
    return query.exec(request);
}
}
