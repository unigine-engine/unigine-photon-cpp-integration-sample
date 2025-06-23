#pragma once

#include <UnigineMathLib.h>
#include <Common-cpp/inc/CustomType.h>

enum EVENT_TYPE
{
	TRANSFORM = 0,
	SHOT,
	HP,
};


class PhotonVec3: public ExitGames::Common::CustomType<PhotonVec3, 1>
{
public:
	using ExitGames::Common::ToString::toString;

	PhotonVec3();
	PhotonVec3(const Unigine::Math::Vec3 &vec);
	~PhotonVec3();

	operator Unigine::Math::Vec3() const { return data; }

	bool compare(const ExitGames::Common::CustomTypeBase &other) const override;
	void duplicate(ExitGames::Common::CustomTypeBase *retVal) const override;
	void deserialize(const nByte *pData, short length) override;
	short serialize(nByte *retVal) const override;

	ExitGames::Common::JString &toString(ExitGames::Common::JString &retStr, bool withTypes = false) const override;

private:
	Unigine::Math::Vec3 data;
};

class PhotonMat4: public ExitGames::Common::CustomType<PhotonMat4, 2>
{
public:
	using ExitGames::Common::ToString::toString;

	PhotonMat4();
	PhotonMat4(const Unigine::Math::Mat4 &mat);
	~PhotonMat4();

	bool compare(const ExitGames::Common::CustomTypeBase &other) const override;
	void duplicate(ExitGames::Common::CustomTypeBase *retVal) const override;
	void deserialize(const nByte *pData, short length) override;
	short serialize(nByte *retVal) const override;

	ExitGames::Common::JString &toString(ExitGames::Common::JString &retStr, bool withTypes = false) const override;

	Unigine::Math::Mat4 getData() { return data; }

private:
	Unigine::Math::Mat4 data;
};
