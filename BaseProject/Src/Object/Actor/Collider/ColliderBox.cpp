#include "ColliderBox.h"
#include<DxLib.h>


ColliderBox::ColliderBox(ColliderInfo& info,  VECTOR& halfSize, ActorBase* actor)
	:
	ColliderBase(info, actor),
	halfSize_(halfSize)
{
}


ColliderBox::~ColliderBox(void)
{
}

VECTOR ColliderBox::Local2World(const VECTOR& localPos) const
{
	VECTOR center = GetRotPos(colliderInfo_.localPos);

	return VAdd(
		center,
		VAdd(
			VAdd(
				VScale(GetAxisX(), localPos.x),
				VScale(GetAxisY(), localPos.y)),
			VScale(GetAxisZ(), localPos.z)
		)
	);
}


VECTOR ColliderBox::World2Local(const VECTOR& worldPos) const
{
	VECTOR center = GetRotPos(colliderInfo_.localPos);
	VECTOR dir = VSub(worldPos, center);

	return VGet(
		VDot(dir, GetAxisX()),
		VDot(dir, GetAxisY()),
		VDot(dir, GetAxisZ())
	);
}


VECTOR ColliderBox::GetVertexPos(int index) const
{
	// 中心座標
	VECTOR center = GetRotPos(colliderInfo_.localPos);

	// ワールド軸
	VECTOR axisX = GetFollow()->GetRight();
	VECTOR axisY = GetFollow()->GetUp();
	VECTOR axisZ = GetFollow()->GetForward();

	// 8頂点の配列
	VECTOR hx = VScale(axisX, halfSize_.x);
	VECTOR hy = VScale(axisY, halfSize_.y);
	VECTOR hz = VScale(axisZ, halfSize_.z);

	VECTOR v[8] =
	{
		VAdd(center,VAdd(VAdd(hx,hy),hz)),              // +x,+y,+z
		VAdd(center,VAdd(VAdd(hx,hy),VScale(hz,-1))),   // +x,+y,-z
		VAdd(center,VAdd(VAdd(hx,VScale(hy,-1)),hz)),   // +x,-y,+z
		VAdd(center,VAdd(VAdd(hx,VScale(hy,-1)),VScale(hz,-1))), // +x,-y,-z
		VAdd(center,VAdd(VAdd(VScale(hx,-1),hy),hz)),  // -x,+y,+z
		VAdd(center,VAdd(VAdd(VScale(hx,-1),hy),VScale(hz,-1))), // -x,+y,-z
		VAdd(center,VAdd(VAdd(VScale(hx,-1),VScale(hy,-1)),hz)), // -x,-y,+z
		VAdd(center,VAdd(VAdd(VScale(hx,-1),VScale(hy,-1)),VScale(hz,-1))) // -x,-y,-z
	};

	if (index < 0 || index > 7) return center; // 範囲外なら中心返す
	return v[index];
}


void ColliderBox::DrawDebug(int color)
{
	//中心座標
	VECTOR center = GetRotPos(colliderInfo_.localPos);
	//ワールド軸
	VECTOR axisX = GetFollow()->GetRight();
	VECTOR axisY = GetFollow()->GetUp();
	VECTOR axisZ = GetFollow()->GetForward();
	//中心から頂点までのベクトル
	VECTOR hx = VScale(axisX, halfSize_.x);
	VECTOR hy = VScale(axisY, halfSize_.y);
	VECTOR hz = VScale(axisZ, halfSize_.z);

	// 頂点座標計算
	VECTOR v[8]=
	{
		VAdd(center,VAdd(VAdd(hx,hy),hz)),	// +x,+y,+z
		VAdd(center,VAdd(VAdd(hx,hy),VScale(hz,-1.0f))),	// +x,+y,-z
		VAdd(center,VAdd(VAdd(hx,VScale(hy,-1.0f)),hz)),	// +x,-y,+z
		VAdd(center,VAdd(VAdd(hx,VScale(hy,-1.0f)),VScale(hz,-1.0f))),	// +x,-y,-z
		VAdd(center,VAdd(VAdd(VScale(hx,-1.0f),hy),hz)),	// -x,+y,+z
		VAdd(center,VAdd(VAdd(VScale(hx,-1.0f),hy),VScale(hz,-1.0f))),	// -x,+y,-z
		VAdd(center,VAdd(VAdd(VScale(hx,-1.0f),VScale(hy,-1.0f)),hz)),	// -x,-y,+z
		VAdd(center,VAdd(VAdd(VScale(hx,-1.0f),VScale(hy,-1.0f)),VScale(hz,-1.0f)))	// -x,-y,-z
	};
	//12辺の描画
	unsigned char edges[12][2] =
	{
		{0,1},{0,2},{0,4},	// 0から1,2,4へ
		{1,3},{1,5},		// 1から3,5へ
		{2,3},{2,6},		// 2から3,6へ
		{3,7},				// 3から7へ
		{4,5},{4,6},		// 4から5,6へ
		{5,7},				// 5から7へ
		{6,7}				// 6から7へ
	};
	for (int i = 0; i < 12; i++)
	{
		DrawLine3D(v[edges[i][0]], v[edges[i][1]], color);
	}

	//中心点
	DrawSphere3D(center, 2.0f, 8, color, color, false);
}
