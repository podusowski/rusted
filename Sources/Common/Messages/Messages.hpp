/* this file is autogenerated, do not edit! */

#ifndef COMMON_MESSAGES_HPP
#define COMMON_MESSAGES_HPP

#include <stdexcept>
#include <vector>
#include <string>
#include <ostream>
#include <memory>

#include <RustedCodec/IWriteBuffer.hpp>
#include <RustedCodec/IReadBuffer.hpp>

#include <RustedCodec/RustedAbstractDecoder.hpp>
#include <RustedCodec/RustedAbstractCoder.hpp>

namespace Common 
{
namespace Messages 
{

namespace Id
{
enum Messages
{
	ServerVersionReq,
	ServerVersionResp,
	UserAuthorizationReq,
	UserAuthorizationResp,
	PlayerResourcesStatusReq,
	PlayerResourcesStatusResp,
	RustedTimeEpochReq,
	RustedTimeEpochResp,
	PlayerEntitiesStatusReq,
	PlayerEntitiesStatusResp,
	EntityGetInfoReq,
	EntityGetInfoResp,
	EntityChangeCourseReq,
	StaticObjectStatusReq,
	StaticObjectStatusResp,
	StaticObjectInfoReq,
	StaticObjectInfoResp,
	DUMMY // message generator issue, don't ask why
};
}

struct AbstractMessage
{
	virtual Id::Messages getId() const = 0 ;
	virtual void serialize(::Common::RustedCodec::IWriteBuffer & buf) = 0;
};

struct ServerVersionReq : public AbstractMessage
{
	Id::Messages getId() const { return Id::ServerVersionReq; }

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::ServerVersionReq
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const ServerVersionReq & msg)
{
	os
		<< "ServerVersionReq\n{\n"		<< "}\n";
	return os;
}

struct ServerVersionResp : public AbstractMessage
{
	Id::Messages getId() const { return Id::ServerVersionResp; }
	std::string version;

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::ServerVersionResp
			<< version
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const ServerVersionResp & msg)
{
	os
		<< "ServerVersionResp\n{\n"		<< "  version: " << msg.version << "\n"
		<< "}\n";
	return os;
}

struct UserAuthorizationReq : public AbstractMessage
{
	Id::Messages getId() const { return Id::UserAuthorizationReq; }
	std::string login;
	std::string password;

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::UserAuthorizationReq
			<< login
			<< password
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const UserAuthorizationReq & msg)
{
	os
		<< "UserAuthorizationReq\n{\n"		<< "  login: " << msg.login << "\n"
		<< "  password: " << msg.password << "\n"
		<< "}\n";
	return os;
}

struct UserAuthorizationResp : public AbstractMessage
{
	Id::Messages getId() const { return Id::UserAuthorizationResp; }
	bool success;
	int player_id;

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::UserAuthorizationResp
			<< success
			<< player_id
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const UserAuthorizationResp & msg)
{
	os
		<< "UserAuthorizationResp\n{\n"		<< "  success: " << msg.success << "\n"
		<< "  player_id: " << msg.player_id << "\n"
		<< "}\n";
	return os;
}

struct PlayerResourcesStatusReq : public AbstractMessage
{
	Id::Messages getId() const { return Id::PlayerResourcesStatusReq; }

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::PlayerResourcesStatusReq
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const PlayerResourcesStatusReq & msg)
{
	os
		<< "PlayerResourcesStatusReq\n{\n"		<< "}\n";
	return os;
}

struct PlayerResourcesStatusResp : public AbstractMessage
{
	Id::Messages getId() const { return Id::PlayerResourcesStatusResp; }
	int carbon;
	int uranium;
	int credits;

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::PlayerResourcesStatusResp
			<< carbon
			<< uranium
			<< credits
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const PlayerResourcesStatusResp & msg)
{
	os
		<< "PlayerResourcesStatusResp\n{\n"		<< "  carbon: " << msg.carbon << "\n"
		<< "  uranium: " << msg.uranium << "\n"
		<< "  credits: " << msg.credits << "\n"
		<< "}\n";
	return os;
}

struct RustedTimeEpochReq : public AbstractMessage
{
	Id::Messages getId() const { return Id::RustedTimeEpochReq; }

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::RustedTimeEpochReq
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const RustedTimeEpochReq & msg)
{
	os
		<< "RustedTimeEpochReq\n{\n"		<< "}\n";
	return os;
}

struct RustedTimeEpochResp : public AbstractMessage
{
	Id::Messages getId() const { return Id::RustedTimeEpochResp; }
	int time;

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::RustedTimeEpochResp
			<< time
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const RustedTimeEpochResp & msg)
{
	os
		<< "RustedTimeEpochResp\n{\n"		<< "  time: " << msg.time << "\n"
		<< "}\n";
	return os;
}

struct PlayerEntitiesStatusReq : public AbstractMessage
{
	Id::Messages getId() const { return Id::PlayerEntitiesStatusReq; }

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::PlayerEntitiesStatusReq
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const PlayerEntitiesStatusReq & msg)
{
	os
		<< "PlayerEntitiesStatusReq\n{\n"		<< "}\n";
	return os;
}

struct PlayerEntitiesStatusResp : public AbstractMessage
{
	Id::Messages getId() const { return Id::PlayerEntitiesStatusResp; }
	std::vector<boost::tuple<int> > entities;

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::PlayerEntitiesStatusResp
			<< entities
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const PlayerEntitiesStatusResp & msg)
{
	os
		<< "PlayerEntitiesStatusResp\n{\n"		<< "}\n";
	return os;
}

struct EntityGetInfoReq : public AbstractMessage
{
	Id::Messages getId() const { return Id::EntityGetInfoReq; }
	int id;

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::EntityGetInfoReq
			<< id
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const EntityGetInfoReq & msg)
{
	os
		<< "EntityGetInfoReq\n{\n"		<< "  id: " << msg.id << "\n"
		<< "}\n";
	return os;
}

struct EntityGetInfoResp : public AbstractMessage
{
	Id::Messages getId() const { return Id::EntityGetInfoResp; }
	int id;
	int player_id;
	int x;
	int y;
	int z;

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::EntityGetInfoResp
			<< id
			<< player_id
			<< x
			<< y
			<< z
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const EntityGetInfoResp & msg)
{
	os
		<< "EntityGetInfoResp\n{\n"		<< "  id: " << msg.id << "\n"
		<< "  player_id: " << msg.player_id << "\n"
		<< "  x: " << msg.x << "\n"
		<< "  y: " << msg.y << "\n"
		<< "  z: " << msg.z << "\n"
		<< "}\n";
	return os;
}

struct EntityChangeCourseReq : public AbstractMessage
{
	Id::Messages getId() const { return Id::EntityChangeCourseReq; }
	int entityId;
	int courseX;
	int courseY;
	int courseZ;

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::EntityChangeCourseReq
			<< entityId
			<< courseX
			<< courseY
			<< courseZ
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const EntityChangeCourseReq & msg)
{
	os
		<< "EntityChangeCourseReq\n{\n"		<< "  entityId: " << msg.entityId << "\n"
		<< "  courseX: " << msg.courseX << "\n"
		<< "  courseY: " << msg.courseY << "\n"
		<< "  courseZ: " << msg.courseZ << "\n"
		<< "}\n";
	return os;
}

struct StaticObjectStatusReq : public AbstractMessage
{
	Id::Messages getId() const { return Id::StaticObjectStatusReq; }

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::StaticObjectStatusReq
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const StaticObjectStatusReq & msg)
{
	os
		<< "StaticObjectStatusReq\n{\n"		<< "}\n";
	return os;
}

struct StaticObjectStatusResp : public AbstractMessage
{
	Id::Messages getId() const { return Id::StaticObjectStatusResp; }
	std::vector<boost::tuple<int> > objects;

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::StaticObjectStatusResp
			<< objects
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const StaticObjectStatusResp & msg)
{
	os
		<< "StaticObjectStatusResp\n{\n"		<< "}\n";
	return os;
}

struct StaticObjectInfoReq : public AbstractMessage
{
	Id::Messages getId() const { return Id::StaticObjectInfoReq; }
	int staticObjectId;

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::StaticObjectInfoReq
			<< staticObjectId
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const StaticObjectInfoReq & msg)
{
	os
		<< "StaticObjectInfoReq\n{\n"		<< "  staticObjectId: " << msg.staticObjectId << "\n"
		<< "}\n";
	return os;
}

struct StaticObjectInfoResp : public AbstractMessage
{
	Id::Messages getId() const { return Id::StaticObjectInfoResp; }
	int staticObjectId;
	int x;
	int y;
	int z;

	void serialize(Common::RustedCodec::IWriteBuffer & buf)
	{
		Common::RustedCodec::RustedAbstractCoder coder(buf);
		coder
			<< Id::StaticObjectInfoResp
			<< staticObjectId
			<< x
			<< y
			<< z
		;
	}

};


inline std::ostream & operator<<(std::ostream & os, const StaticObjectInfoResp & msg)
{
	os
		<< "StaticObjectInfoResp\n{\n"		<< "  staticObjectId: " << msg.staticObjectId << "\n"
		<< "  x: " << msg.x << "\n"
		<< "  y: " << msg.y << "\n"
		<< "  z: " << msg.z << "\n"
		<< "}\n";
	return os;
}

class MessageFactory
{
public:
	static std::auto_ptr<AbstractMessage> create(::Common::RustedCodec::IReadBuffer & buf)
	{
		::Common::RustedCodec::RustedAbstractDecoder decoder(buf);
		std::auto_ptr<AbstractMessage> message;
		int id;
		decoder >> id;
		switch (id)
		{
			case Id::ServerVersionReq:
				message.reset(new ServerVersionReq());
				break;
			case Id::ServerVersionResp:
				message.reset(new ServerVersionResp());
				decoder
					>> static_cast<ServerVersionResp *>(message.get())->version
				;
				break;
			case Id::UserAuthorizationReq:
				message.reset(new UserAuthorizationReq());
				decoder
					>> static_cast<UserAuthorizationReq *>(message.get())->login
					>> static_cast<UserAuthorizationReq *>(message.get())->password
				;
				break;
			case Id::UserAuthorizationResp:
				message.reset(new UserAuthorizationResp());
				decoder
					>> static_cast<UserAuthorizationResp *>(message.get())->success
					>> static_cast<UserAuthorizationResp *>(message.get())->player_id
				;
				break;
			case Id::PlayerResourcesStatusReq:
				message.reset(new PlayerResourcesStatusReq());
				break;
			case Id::PlayerResourcesStatusResp:
				message.reset(new PlayerResourcesStatusResp());
				decoder
					>> static_cast<PlayerResourcesStatusResp *>(message.get())->carbon
					>> static_cast<PlayerResourcesStatusResp *>(message.get())->uranium
					>> static_cast<PlayerResourcesStatusResp *>(message.get())->credits
				;
				break;
			case Id::RustedTimeEpochReq:
				message.reset(new RustedTimeEpochReq());
				break;
			case Id::RustedTimeEpochResp:
				message.reset(new RustedTimeEpochResp());
				decoder
					>> static_cast<RustedTimeEpochResp *>(message.get())->time
				;
				break;
			case Id::PlayerEntitiesStatusReq:
				message.reset(new PlayerEntitiesStatusReq());
				break;
			case Id::PlayerEntitiesStatusResp:
				message.reset(new PlayerEntitiesStatusResp());
				decoder
					>> static_cast<PlayerEntitiesStatusResp *>(message.get())->entities
				;
				break;
			case Id::EntityGetInfoReq:
				message.reset(new EntityGetInfoReq());
				decoder
					>> static_cast<EntityGetInfoReq *>(message.get())->id
				;
				break;
			case Id::EntityGetInfoResp:
				message.reset(new EntityGetInfoResp());
				decoder
					>> static_cast<EntityGetInfoResp *>(message.get())->id
					>> static_cast<EntityGetInfoResp *>(message.get())->player_id
					>> static_cast<EntityGetInfoResp *>(message.get())->x
					>> static_cast<EntityGetInfoResp *>(message.get())->y
					>> static_cast<EntityGetInfoResp *>(message.get())->z
				;
				break;
			case Id::EntityChangeCourseReq:
				message.reset(new EntityChangeCourseReq());
				decoder
					>> static_cast<EntityChangeCourseReq *>(message.get())->entityId
					>> static_cast<EntityChangeCourseReq *>(message.get())->courseX
					>> static_cast<EntityChangeCourseReq *>(message.get())->courseY
					>> static_cast<EntityChangeCourseReq *>(message.get())->courseZ
				;
				break;
			case Id::StaticObjectStatusReq:
				message.reset(new StaticObjectStatusReq());
				break;
			case Id::StaticObjectStatusResp:
				message.reset(new StaticObjectStatusResp());
				decoder
					>> static_cast<StaticObjectStatusResp *>(message.get())->objects
				;
				break;
			case Id::StaticObjectInfoReq:
				message.reset(new StaticObjectInfoReq());
				decoder
					>> static_cast<StaticObjectInfoReq *>(message.get())->staticObjectId
				;
				break;
			case Id::StaticObjectInfoResp:
				message.reset(new StaticObjectInfoResp());
				decoder
					>> static_cast<StaticObjectInfoResp *>(message.get())->staticObjectId
					>> static_cast<StaticObjectInfoResp *>(message.get())->x
					>> static_cast<StaticObjectInfoResp *>(message.get())->y
					>> static_cast<StaticObjectInfoResp *>(message.get())->z
				;
				break;
			default: throw std::runtime_error("unknown message id");
		}
		return message;
	}
};

template <typename HandlerType>
class HandlerCaller0
{
public:
	HandlerCaller0(HandlerType & handler) : m_handler(handler) {}
	void call(AbstractMessage & message)
	{
		switch (message.getId())
		{
			case Id::ServerVersionReq: m_handler.handle( static_cast<const ServerVersionReq &>(message)); break;
			case Id::ServerVersionResp: m_handler.handle( static_cast<const ServerVersionResp &>(message)); break;
			case Id::UserAuthorizationReq: m_handler.handle( static_cast<const UserAuthorizationReq &>(message)); break;
			case Id::UserAuthorizationResp: m_handler.handle( static_cast<const UserAuthorizationResp &>(message)); break;
			case Id::PlayerResourcesStatusReq: m_handler.handle( static_cast<const PlayerResourcesStatusReq &>(message)); break;
			case Id::PlayerResourcesStatusResp: m_handler.handle( static_cast<const PlayerResourcesStatusResp &>(message)); break;
			case Id::RustedTimeEpochReq: m_handler.handle( static_cast<const RustedTimeEpochReq &>(message)); break;
			case Id::RustedTimeEpochResp: m_handler.handle( static_cast<const RustedTimeEpochResp &>(message)); break;
			case Id::PlayerEntitiesStatusReq: m_handler.handle( static_cast<const PlayerEntitiesStatusReq &>(message)); break;
			case Id::PlayerEntitiesStatusResp: m_handler.handle( static_cast<const PlayerEntitiesStatusResp &>(message)); break;
			case Id::EntityGetInfoReq: m_handler.handle( static_cast<const EntityGetInfoReq &>(message)); break;
			case Id::EntityGetInfoResp: m_handler.handle( static_cast<const EntityGetInfoResp &>(message)); break;
			case Id::EntityChangeCourseReq: m_handler.handle( static_cast<const EntityChangeCourseReq &>(message)); break;
			case Id::StaticObjectStatusReq: m_handler.handle( static_cast<const StaticObjectStatusReq &>(message)); break;
			case Id::StaticObjectStatusResp: m_handler.handle( static_cast<const StaticObjectStatusResp &>(message)); break;
			case Id::StaticObjectInfoReq: m_handler.handle( static_cast<const StaticObjectInfoReq &>(message)); break;
			case Id::StaticObjectInfoResp: m_handler.handle( static_cast<const StaticObjectInfoResp &>(message)); break;
			default: throw std::exception();
		}
	}

private:
	HandlerType & m_handler;
};

template <typename HandlerType, typename T0>
class HandlerCaller1
{
public:
	HandlerCaller1(HandlerType & handler) : m_handler(handler) {}
	void call(AbstractMessage & message, T0 p0)
	{
		switch (message.getId())
		{
			case Id::ServerVersionReq: m_handler.handle( static_cast<const ServerVersionReq &>(message), p0); break;
			case Id::ServerVersionResp: m_handler.handle( static_cast<const ServerVersionResp &>(message), p0); break;
			case Id::UserAuthorizationReq: m_handler.handle( static_cast<const UserAuthorizationReq &>(message), p0); break;
			case Id::UserAuthorizationResp: m_handler.handle( static_cast<const UserAuthorizationResp &>(message), p0); break;
			case Id::PlayerResourcesStatusReq: m_handler.handle( static_cast<const PlayerResourcesStatusReq &>(message), p0); break;
			case Id::PlayerResourcesStatusResp: m_handler.handle( static_cast<const PlayerResourcesStatusResp &>(message), p0); break;
			case Id::RustedTimeEpochReq: m_handler.handle( static_cast<const RustedTimeEpochReq &>(message), p0); break;
			case Id::RustedTimeEpochResp: m_handler.handle( static_cast<const RustedTimeEpochResp &>(message), p0); break;
			case Id::PlayerEntitiesStatusReq: m_handler.handle( static_cast<const PlayerEntitiesStatusReq &>(message), p0); break;
			case Id::PlayerEntitiesStatusResp: m_handler.handle( static_cast<const PlayerEntitiesStatusResp &>(message), p0); break;
			case Id::EntityGetInfoReq: m_handler.handle( static_cast<const EntityGetInfoReq &>(message), p0); break;
			case Id::EntityGetInfoResp: m_handler.handle( static_cast<const EntityGetInfoResp &>(message), p0); break;
			case Id::EntityChangeCourseReq: m_handler.handle( static_cast<const EntityChangeCourseReq &>(message), p0); break;
			case Id::StaticObjectStatusReq: m_handler.handle( static_cast<const StaticObjectStatusReq &>(message), p0); break;
			case Id::StaticObjectStatusResp: m_handler.handle( static_cast<const StaticObjectStatusResp &>(message), p0); break;
			case Id::StaticObjectInfoReq: m_handler.handle( static_cast<const StaticObjectInfoReq &>(message), p0); break;
			case Id::StaticObjectInfoResp: m_handler.handle( static_cast<const StaticObjectInfoResp &>(message), p0); break;
			default: throw std::exception();
		}
	}

private:
	HandlerType & m_handler;
};

template <typename HandlerType, typename T0, typename T1>
class HandlerCaller2
{
public:
	HandlerCaller2(HandlerType & handler) : m_handler(handler) {}
	void call(AbstractMessage & message, T0 p0, T1 p1)
	{
		switch (message.getId())
		{
			case Id::ServerVersionReq: m_handler.handle( static_cast<const ServerVersionReq &>(message), p0, p1); break;
			case Id::ServerVersionResp: m_handler.handle( static_cast<const ServerVersionResp &>(message), p0, p1); break;
			case Id::UserAuthorizationReq: m_handler.handle( static_cast<const UserAuthorizationReq &>(message), p0, p1); break;
			case Id::UserAuthorizationResp: m_handler.handle( static_cast<const UserAuthorizationResp &>(message), p0, p1); break;
			case Id::PlayerResourcesStatusReq: m_handler.handle( static_cast<const PlayerResourcesStatusReq &>(message), p0, p1); break;
			case Id::PlayerResourcesStatusResp: m_handler.handle( static_cast<const PlayerResourcesStatusResp &>(message), p0, p1); break;
			case Id::RustedTimeEpochReq: m_handler.handle( static_cast<const RustedTimeEpochReq &>(message), p0, p1); break;
			case Id::RustedTimeEpochResp: m_handler.handle( static_cast<const RustedTimeEpochResp &>(message), p0, p1); break;
			case Id::PlayerEntitiesStatusReq: m_handler.handle( static_cast<const PlayerEntitiesStatusReq &>(message), p0, p1); break;
			case Id::PlayerEntitiesStatusResp: m_handler.handle( static_cast<const PlayerEntitiesStatusResp &>(message), p0, p1); break;
			case Id::EntityGetInfoReq: m_handler.handle( static_cast<const EntityGetInfoReq &>(message), p0, p1); break;
			case Id::EntityGetInfoResp: m_handler.handle( static_cast<const EntityGetInfoResp &>(message), p0, p1); break;
			case Id::EntityChangeCourseReq: m_handler.handle( static_cast<const EntityChangeCourseReq &>(message), p0, p1); break;
			case Id::StaticObjectStatusReq: m_handler.handle( static_cast<const StaticObjectStatusReq &>(message), p0, p1); break;
			case Id::StaticObjectStatusResp: m_handler.handle( static_cast<const StaticObjectStatusResp &>(message), p0, p1); break;
			case Id::StaticObjectInfoReq: m_handler.handle( static_cast<const StaticObjectInfoReq &>(message), p0, p1); break;
			case Id::StaticObjectInfoResp: m_handler.handle( static_cast<const StaticObjectInfoResp &>(message), p0, p1); break;
			default: throw std::exception();
		}
	}

private:
	HandlerType & m_handler;
};

template <typename HandlerType, typename T0, typename T1, typename T2>
class HandlerCaller3
{
public:
	HandlerCaller3(HandlerType & handler) : m_handler(handler) {}
	void call(AbstractMessage & message, T0 p0, T1 p1, T2 p2)
	{
		switch (message.getId())
		{
			case Id::ServerVersionReq: m_handler.handle( static_cast<const ServerVersionReq &>(message), p0, p1, p2); break;
			case Id::ServerVersionResp: m_handler.handle( static_cast<const ServerVersionResp &>(message), p0, p1, p2); break;
			case Id::UserAuthorizationReq: m_handler.handle( static_cast<const UserAuthorizationReq &>(message), p0, p1, p2); break;
			case Id::UserAuthorizationResp: m_handler.handle( static_cast<const UserAuthorizationResp &>(message), p0, p1, p2); break;
			case Id::PlayerResourcesStatusReq: m_handler.handle( static_cast<const PlayerResourcesStatusReq &>(message), p0, p1, p2); break;
			case Id::PlayerResourcesStatusResp: m_handler.handle( static_cast<const PlayerResourcesStatusResp &>(message), p0, p1, p2); break;
			case Id::RustedTimeEpochReq: m_handler.handle( static_cast<const RustedTimeEpochReq &>(message), p0, p1, p2); break;
			case Id::RustedTimeEpochResp: m_handler.handle( static_cast<const RustedTimeEpochResp &>(message), p0, p1, p2); break;
			case Id::PlayerEntitiesStatusReq: m_handler.handle( static_cast<const PlayerEntitiesStatusReq &>(message), p0, p1, p2); break;
			case Id::PlayerEntitiesStatusResp: m_handler.handle( static_cast<const PlayerEntitiesStatusResp &>(message), p0, p1, p2); break;
			case Id::EntityGetInfoReq: m_handler.handle( static_cast<const EntityGetInfoReq &>(message), p0, p1, p2); break;
			case Id::EntityGetInfoResp: m_handler.handle( static_cast<const EntityGetInfoResp &>(message), p0, p1, p2); break;
			case Id::EntityChangeCourseReq: m_handler.handle( static_cast<const EntityChangeCourseReq &>(message), p0, p1, p2); break;
			case Id::StaticObjectStatusReq: m_handler.handle( static_cast<const StaticObjectStatusReq &>(message), p0, p1, p2); break;
			case Id::StaticObjectStatusResp: m_handler.handle( static_cast<const StaticObjectStatusResp &>(message), p0, p1, p2); break;
			case Id::StaticObjectInfoReq: m_handler.handle( static_cast<const StaticObjectInfoReq &>(message), p0, p1, p2); break;
			case Id::StaticObjectInfoResp: m_handler.handle( static_cast<const StaticObjectInfoResp &>(message), p0, p1, p2); break;
			default: throw std::exception();
		}
	}

private:
	HandlerType & m_handler;
};

template <typename HandlerType, typename T0, typename T1, typename T2, typename T3>
class HandlerCaller4
{
public:
	HandlerCaller4(HandlerType & handler) : m_handler(handler) {}
	void call(AbstractMessage & message, T0 p0, T1 p1, T2 p2, T3 p3)
	{
		switch (message.getId())
		{
			case Id::ServerVersionReq: m_handler.handle( static_cast<const ServerVersionReq &>(message), p0, p1, p2, p3); break;
			case Id::ServerVersionResp: m_handler.handle( static_cast<const ServerVersionResp &>(message), p0, p1, p2, p3); break;
			case Id::UserAuthorizationReq: m_handler.handle( static_cast<const UserAuthorizationReq &>(message), p0, p1, p2, p3); break;
			case Id::UserAuthorizationResp: m_handler.handle( static_cast<const UserAuthorizationResp &>(message), p0, p1, p2, p3); break;
			case Id::PlayerResourcesStatusReq: m_handler.handle( static_cast<const PlayerResourcesStatusReq &>(message), p0, p1, p2, p3); break;
			case Id::PlayerResourcesStatusResp: m_handler.handle( static_cast<const PlayerResourcesStatusResp &>(message), p0, p1, p2, p3); break;
			case Id::RustedTimeEpochReq: m_handler.handle( static_cast<const RustedTimeEpochReq &>(message), p0, p1, p2, p3); break;
			case Id::RustedTimeEpochResp: m_handler.handle( static_cast<const RustedTimeEpochResp &>(message), p0, p1, p2, p3); break;
			case Id::PlayerEntitiesStatusReq: m_handler.handle( static_cast<const PlayerEntitiesStatusReq &>(message), p0, p1, p2, p3); break;
			case Id::PlayerEntitiesStatusResp: m_handler.handle( static_cast<const PlayerEntitiesStatusResp &>(message), p0, p1, p2, p3); break;
			case Id::EntityGetInfoReq: m_handler.handle( static_cast<const EntityGetInfoReq &>(message), p0, p1, p2, p3); break;
			case Id::EntityGetInfoResp: m_handler.handle( static_cast<const EntityGetInfoResp &>(message), p0, p1, p2, p3); break;
			case Id::EntityChangeCourseReq: m_handler.handle( static_cast<const EntityChangeCourseReq &>(message), p0, p1, p2, p3); break;
			case Id::StaticObjectStatusReq: m_handler.handle( static_cast<const StaticObjectStatusReq &>(message), p0, p1, p2, p3); break;
			case Id::StaticObjectStatusResp: m_handler.handle( static_cast<const StaticObjectStatusResp &>(message), p0, p1, p2, p3); break;
			case Id::StaticObjectInfoReq: m_handler.handle( static_cast<const StaticObjectInfoReq &>(message), p0, p1, p2, p3); break;
			case Id::StaticObjectInfoResp: m_handler.handle( static_cast<const StaticObjectInfoResp &>(message), p0, p1, p2, p3); break;
			default: throw std::exception();
		}
	}

private:
	HandlerType & m_handler;
};


inline std::ostream & operator<<(std::ostream & os, const AbstractMessage & msg)
{
	switch (msg.getId())
	{
		case Id::ServerVersionReq: os << static_cast<const ServerVersionReq &>(msg); break;
		case Id::ServerVersionResp: os << static_cast<const ServerVersionResp &>(msg); break;
		case Id::UserAuthorizationReq: os << static_cast<const UserAuthorizationReq &>(msg); break;
		case Id::UserAuthorizationResp: os << static_cast<const UserAuthorizationResp &>(msg); break;
		case Id::PlayerResourcesStatusReq: os << static_cast<const PlayerResourcesStatusReq &>(msg); break;
		case Id::PlayerResourcesStatusResp: os << static_cast<const PlayerResourcesStatusResp &>(msg); break;
		case Id::RustedTimeEpochReq: os << static_cast<const RustedTimeEpochReq &>(msg); break;
		case Id::RustedTimeEpochResp: os << static_cast<const RustedTimeEpochResp &>(msg); break;
		case Id::PlayerEntitiesStatusReq: os << static_cast<const PlayerEntitiesStatusReq &>(msg); break;
		case Id::PlayerEntitiesStatusResp: os << static_cast<const PlayerEntitiesStatusResp &>(msg); break;
		case Id::EntityGetInfoReq: os << static_cast<const EntityGetInfoReq &>(msg); break;
		case Id::EntityGetInfoResp: os << static_cast<const EntityGetInfoResp &>(msg); break;
		case Id::EntityChangeCourseReq: os << static_cast<const EntityChangeCourseReq &>(msg); break;
		case Id::StaticObjectStatusReq: os << static_cast<const StaticObjectStatusReq &>(msg); break;
		case Id::StaticObjectStatusResp: os << static_cast<const StaticObjectStatusResp &>(msg); break;
		case Id::StaticObjectInfoReq: os << static_cast<const StaticObjectInfoReq &>(msg); break;
		case Id::StaticObjectInfoResp: os << static_cast<const StaticObjectInfoResp &>(msg); break;
	default: throw std::exception();
	}
	return os;
}

}
}

#endif
