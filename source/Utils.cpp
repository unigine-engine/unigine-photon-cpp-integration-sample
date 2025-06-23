#include "Utils.h"
#include <cstring>

using namespace Unigine;
using namespace Math;
using namespace ExitGames;

PhotonVec3::PhotonVec3()
{
}

PhotonVec3::PhotonVec3(const Vec3 &vec)
{
	data = vec;
}

PhotonVec3::~PhotonVec3() {}

bool PhotonVec3::compare(const Common::CustomTypeBase &other) const
{
	return data.v == static_cast<const PhotonVec3 &>(other).data.v;
}

void PhotonVec3::duplicate(Common::CustomTypeBase *retVal) const
{
	*reinterpret_cast<PhotonVec3 *>(retVal) = *this;
}

void PhotonVec3::deserialize(const nByte *pData, short length)
{
	if (length != sizeof(data.v))
	{
		return;
	}

	std::memcpy(data.v, pData, length);
}

short PhotonVec3::serialize(nByte *retVal) const
{
	if (retVal)
	{
		std::memcpy(retVal, data.v, sizeof(data.v));
	}

	return sizeof(data.v);
}

ExitGames::Common::JString &PhotonVec3::toString(ExitGames::Common::JString &retStr, bool withTypes) const
{
	return retStr = Common::JString(L"(") + data.x + L", " + data.y + L", " + data.z + L")";
}

PhotonMat4::PhotonMat4()
{
}

PhotonMat4::PhotonMat4(const Mat4 &mat)
{
	data = mat;
}

PhotonMat4::~PhotonMat4() {}

bool PhotonMat4::compare(const ExitGames::Common::CustomTypeBase &other) const
{
	return data.mat == static_cast<const PhotonMat4 &>(other).data.mat;
}

void PhotonMat4::duplicate(ExitGames::Common::CustomTypeBase *retVal) const
{
	*reinterpret_cast<PhotonMat4 *>(retVal) = *this;
}

void PhotonMat4::deserialize(const nByte *pData, short length)
{
	if (length != sizeof(data.mat))
	{
		return;
	}

	std::memcpy(data.mat, pData, length);
}

short PhotonMat4::serialize(nByte *retVal) const
{
	if (retVal)
	{
		std::memcpy(retVal, data.mat, sizeof(data.mat));
	}

	return sizeof(data.mat);
}

ExitGames::Common::JString &PhotonMat4::toString(ExitGames::Common::JString &retStr, bool withTypes) const
{
	return retStr = Common::JString(L"(") + data.m00 + L", " + data.m01 + L", " + data.m02 + L", " + data.m03 + L")\n" +
									L"(" + data.m10 + L", " + data.m11 + L", " + data.m12 + L", " + data.m13 + L")\n" +
									L"(" + data.m20 + L", " + data.m21 + L", " + data.m22 + L", " + data.m23 + L")\n";
}
