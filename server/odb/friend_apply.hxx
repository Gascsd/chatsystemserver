#pragma once
#include <string>
#include <cstddef> // std::size_t
#include <odb/nullable.hxx>
#include <odb/core.hxx>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace zht_im
{
#pragma db object table("friend_apply")
    class FriendApply
    {
        friend class odb::access;

    public:
        FriendApply() {}

        FriendApply(const std::string &eid, const std::string &uid, const std::string &pid)
            : _event_id(eid), _user_id(uid), _peer_id(pid) {}

        std::string event_id() const { return _event_id; }
        void event_id(const std::string &eid) { _event_id = eid; }

        std::string user_id() const { return _user_id; }
        void user_id(const std::string &uid) { _user_id = uid; }

        std::string peer_id() const { return _peer_id; }
        void peer_id(const std::string &pid) { _peer_id = pid; }

    private:
#pragma db id auto
        unsigned long _id;
#pragma db type("varchar(64)") index unique
        std::string _event_id;
#pragma db type("varchar(64)") index
        std::string _user_id;
#pragma db type("varchar(64)") index
        std::string _peer_id;
    };
}