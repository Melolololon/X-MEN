#include "RouteSearch.h"
#include"LibMath.h"
#include"Collision.h"
#include"ModelData.h"

using namespace MelLib;

//#ifdef _DEBUG
//std::vector<AStarNode*>AStarNode::pNodes;
//
//MelLib::AStarNode::AStarNode()
//{
//	pNodes.push_back(this);
//
//	//角度設定めんどいからボックスにしてる
//	nodeModel.Create(ModelData::Get(ShapeType3D::BOX), nullptr);
//	//nodeModel.SetScale(size)
//}
//
//MelLib::AStarNode::~AStarNode()
//{
//	for (int i = 0, size = pNodes.size(); i < size; i++) 
//	{
//		if (pNodes[i] == this) 
//		{
//			pNodes.erase(pNodes.begin() + i);
//			break;
//		}
//	}
//}
//
//void MelLib::AStarNode::Draw()
//{
//	nodeModel.Draw();
//}
//
//void MelLib::AStarNode::AllDraw()
//{
//	for(auto& p : pNodes)
//	{
//		p->Draw();
//	}
//}

//#endif // _DEBUG


#pragma region 最短経路
//
//void RouteSearch::SetAStarNodePosition
//(
//	const Vector2& leftUpPos,
//	const Vector2& rightDownPos,
//	const UINT& nodeNumX,
//	const UINT& nodeNumY,
//	std::vector< std::vector<AStarNode>>& nodes,
//	const bool upPlus
//)
//{
//	nodes.clear();
//
//	//マスのサイズを求める
//	Vector2 size = rightDownPos - leftUpPos;
//	size.x = abs(size.x);
//	size.y = abs(size.y);
//	if (nodeNumX >= 1) size.x /= nodeNumX - 1;
//	if (nodeNumY >= 1) size.y /= nodeNumY - 1;
//
//	//ノードリサイズ
//	nodes.resize(nodeNumY, std::vector<AStarNode>(nodeNumX));
//
//	//ノードの座標をセットしていく
//	Vector2 addPos = 0;
//	for (int y = 0; y < nodeNumY; y++)
//	{
//		for (int x = 0; x < nodeNumX; x++)
//		{
//			if (upPlus)addPos = Vector2(size.x * x, size.y * -y);
//			else addPos = Vector2(size.x * x, size.y * y);
//			nodes[y][x].position = leftUpPos + addPos;
//		}
//	}
//
//}
//
//void RouteSearch::SetAStarNodeHitObjectNodeFlag
//(
//	const std::vector<Vector2>& hitObjectsPos,
//	const std::vector<Vector2>& hitObjectsSize,
//	std::vector< std::vector<AStarNode>>& nodes
//)
//{
//	//サイズを求める
//	Vector2 nodeSize = 0;
//	nodeSize.x = abs(nodes[0][0].position.x - nodes[0][1].position.x);
//
//	float smallPos = 0.0f;
//	float bigPos = 0.0f;
//	if (nodes[0][0].position.y >= nodes[1][0].position.y)
//	{
//		bigPos = nodes[0][0].position.y;
//		smallPos = nodes[1][0].position.y;
//	}
//	else
//	{
//		bigPos = nodes[1][0].position.y;
//		smallPos = nodes[0][0].position.y;
//	}
//	nodeSize.y = abs(smallPos - bigPos);
//
//	//判定
//	auto nodesXNum = nodes[0].size();
//	auto nodesYNum = nodes.size();
//	auto hitObjectNum = hitObjectsPos.size();
//
//	//当たってるか確認
//	for (int y = 0; y < nodesYNum; y++)
//	{
//		for (int x = 0; x < nodesXNum; x++)
//		{
//
//			for (int i = 0; i < hitObjectNum; i++)
//			{
//				nodes[y][x].hitObjectNode =
//					LibMath::RectAndRectCollision
//					(
//						nodes[y][x].position - nodeSize / 2,
//						nodeSize,
//						hitObjectsPos[i] - hitObjectsSize[i] / 2,
//						hitObjectsSize[i]
//					);
//
//				//ぶつかってたらbreak
//				if (nodes[y][x].hitObjectNode)
//				{
//					break;
//				}
//
//			}
//		}
//	}
//}
//
//bool RouteSearch::GetAStarCalcResult
//(
//	const Vector2& startPos,
//	const Vector2& endPos,
//	std::vector< std::vector<AStarNode>>& nodes,
//	std::vector<Vector2>& routeVector
//)
//{
//
//	//やること
//	//ブロックに隣接してるマスから、同じブロックに隣接してるマスへ移動しないようにする
//
//
//	//リセット
//	for (auto& n1 : nodes)
//	{
//		for (auto& n2 : n1)
//		{
//			n2.closeFlag = false;
//			n2.openFlag = false;
//			n2.previousNode = nullptr;
//			n2.calcNum = UINT_MAX;
//			n2.closeIndex = INT_MAX;
//		}
//	}
//
//	//スタート地点に一番近いノードの距離を格納する変数
//	float startMinDistance = FLT_MAX;
//	//ゴール地点に一番近いノードの距離を格納する変数
//	float endMinDistance = FLT_MAX;
//
//	auto nodeXArrayNum = nodes[0].size();
//	auto nodeYArrayNum = nodes.size();
//
//	//スタートの添え字
//	int startNodeIndexX = 0;
//	int startNodeIndexY = 0;
//	bool trueStartNodeHitFlag = false;
//
//	//ノードの配列のゴール地点の場所を示す添え字
//	int endNodeIndexX = 0;
//	int endNodeIndexY = 0;
//	bool trueEndNodeHitFlag = false;
//
//	//一時的にfalseにしたときに戻す処理
//	auto ReturnHitObjectNode = [&]()
//	{
//		if (trueStartNodeHitFlag)nodes[startNodeIndexY][startNodeIndexX].hitObjectNode = true;
//		if (trueEndNodeHitFlag)nodes[endNodeIndexY][endNodeIndexX].hitObjectNode = true;
//
//	};
//
//	for (int y = 0; y < nodeYArrayNum; y++)
//	{
//		for (int x = 0; x < nodeXArrayNum; x++)
//		{
//			float distance = 0.0f;
//			distance = LibMath::CalcDistance2D(nodes[y][x].position, startPos);
//
//			if (distance <= startMinDistance)
//			{
//				startMinDistance = distance;
//				startNodeIndexX = x;
//				startNodeIndexY = y;
//
//			}
//
//			distance = LibMath::CalcDistance2D(nodes[y][x].position, endPos);
//			if (distance <= endMinDistance)
//			{
//				endMinDistance = distance;
//				endNodeIndexX = x;
//				endNodeIndexY = y;
//
//
//			}
//
//			//インデックス代入
//			nodes[y][x].indexX = x;
//			nodes[y][x].indexY = y;
//		}
//	}
//
//	if (nodes[startNodeIndexY][startNodeIndexX].hitObjectNode)
//	{
//		//一時的にfalse
//		nodes[startNodeIndexY][startNodeIndexX].hitObjectNode = false;
//		trueStartNodeHitFlag = true;
//	}
//
//	if (nodes[endNodeIndexY][endNodeIndexX].hitObjectNode)
//	{
//		//一時的にfalse
//		nodes[endNodeIndexY][endNodeIndexX].hitObjectNode = false;
//		trueEndNodeHitFlag = true;
//	}
//
//	//ゴールのノードまでの距離を求めるラムダ式
//	auto CalcNodeDistance = []
//	(
//		const int startX,
//		const int startY,
//		const int endX,
//		const int endY
//		)
//	{
//		//Xの差
//		int startXToEndXDiff = abs(startX - endX);
//		//Yの差
//		int startYToEndYDiff = abs(startY - endY);
//
//		//Xの差とYの差のどちらが大きいか求める。斜め移動ありの場合、大きいほうが最短距離
//		if (startXToEndXDiff >= startYToEndYDiff)return startXToEndXDiff;
//		return startYToEndYDiff;
//
//	};
//
//	//スタートのノードのインデックスを代入
//	int startToEndDis = CalcNodeDistance(startNodeIndexX, startNodeIndexY, endNodeIndexX, endNodeIndexY);
//	nodes[startNodeIndexY][startNodeIndexX].calcNum = startToEndDis + nodes[startNodeIndexY][startNodeIndexX].cost;
//
//	std::vector<AStarNode*>openNodes(1, &nodes[startNodeIndexY][startNodeIndexX]);
//	std::vector<AStarNode*>closeNodes;
//
//
//	AStarNode* endNode;
//
//
//	//ステップの計算は周りのノード調べるときにやる?
//
//	while (1)
//	{
//		//探索終了かどうか
//		bool checkEnd = false;
//
//		//ゴールにたどり着けない場合
//		if (openNodes.size() == 0)
//		{
//			ReturnHitObjectNode();
//			return false;
//		}
//
//		//並び替え
//		std::sort
//		(
//			openNodes.begin(),
//			openNodes.end(),
//			[]
//		(
//			const AStarNode* node1,
//			const AStarNode* node2
//			)
//		{
//			int num1 = node1->calcNum;
//			int num2 = node2->calcNum;
//
//			return num1 > num2;
//		});
//
//		//calcNumが一番少ないやつを取得
//		AStarNode* mainNode = nullptr;
//		int minCalcNum = openNodes[openNodes.size() - 1]->calcNum;
//		float minDis = FLT_MAX;
//		int mainNodeOpenIndex = 0;
//		for (int i = openNodes.size() - 1;; i--)
//		{
//			float dis = LibMath::CalcDistance2D(openNodes[i]->position, nodes[startNodeIndexY][startNodeIndexX].position);
//			if (minDis > dis)
//			{
//				minDis = dis;
//				mainNode = openNodes[i];
//				mainNodeOpenIndex = i;
//			}
//			if (openNodes[i]->calcNum != minCalcNum || i == 0)break;
//		}
//
//
//
//		//オープンに追加するノードを格納する配列
//		std::vector<AStarNode*>openPushBackNode;
//
//		for (int y = -1; y < 2; y++)
//		{
//			int indexY = mainNode->indexY + y;
//
//			//Y範囲外指定防止
//			if (indexY <= -1 || indexY >= nodeYArrayNum)continue;
//
//			for (int x = -1; x < 2; x++)
//			{
//				int indexX = mainNode->indexX + x;
//
//				//X範囲外指定防止
//				if (indexX <= -1 || indexX >= nodeXArrayNum)continue;
//
//				//基準検索防止
//				if (indexX == mainNode->indexX && indexY == mainNode->indexY)continue;
//
//				AStarNode* checkNode = &nodes[indexY][indexX];
//
//				//オブジェクトに重なってるかどうか
//				if (checkNode->hitObjectNode)
//				{
//					checkNode->closeFlag = true;
//					continue;
//				}
//
//
//				//calcNum = ステップ数 + ゴールまでの距離 + コスト
//				int calcNum = mainNode->stepNum + 1
//					+ CalcNodeDistance(indexX, indexY, endNodeIndexX, endNodeIndexY)
//					+ checkNode->cost;
//
//				//条件を満たしたら代入
//				if (checkNode->openFlag)
//				{
//					if (calcNum < checkNode->calcNum)
//					{
//						checkNode->calcNum = calcNum;
//						checkNode->previousNode = mainNode;
//						checkNode->stepNum = mainNode->stepNum + 1;
//					}
//
//					continue;
//				}
//
//				//条件を満たしたら、closeからopenに移動&代入
//				if (checkNode->closeFlag)
//				{
//					if (calcNum < checkNode->calcNum)
//					{
//						//closeから削除
//						closeNodes.erase(closeNodes.begin() + checkNode->closeIndex);
//						checkNode->closeFlag = false;
//						checkNode->closeIndex = INT_MAX;
//
//						//openに追加
//						openNodes.push_back(checkNode);
//						checkNode->openFlag = true;
//
//						checkNode->calcNum = calcNum;
//						checkNode->previousNode = mainNode;
//						checkNode->stepNum = mainNode->stepNum + 1;
//					}
//					continue;
//				}
//
//
//				checkNode->calcNum = calcNum;
//				checkNode->previousNode = mainNode;
//				checkNode->stepNum = mainNode->stepNum + 1;
//
//				if (checkNode->indexX == endNodeIndexX && checkNode->indexY == endNodeIndexY)
//				{
//					checkEnd = true;
//					endNode = checkNode;
//				}
//
//
//				openPushBackNode.push_back(checkNode);
//				checkNode->openFlag = true;
//			}
//		}
//
//		if (checkEnd)break;
//
//		closeNodes.push_back(mainNode);
//		mainNode->closeFlag = true;
//		mainNode->closeIndex = closeNodes.size() - 1;
//
//		//検索したやつを消す
//		openNodes[openNodes.size() - 1]->openFlag = false;
//		openNodes.erase(openNodes.begin() + mainNodeOpenIndex);
//
//
//		for (auto& n : openPushBackNode)
//		{
//			openNodes.push_back(n);
//		}
//		openPushBackNode.clear();
//	}
//
//	AStarNode* currentNode = endNode;
//	std::vector<Vector2>routeNodeVectors;
//	while (1)
//	{
//		if (!currentNode)break;
//		routeNodeVectors.push_back(currentNode->position);
//		currentNode = currentNode->previousNode;
//	}
//	std::reverse(routeNodeVectors.begin(), routeNodeVectors.end());
//
//	//ベクトルを求める
//	routeVector.clear();
//	auto routeVectorSize = routeNodeVectors.size() - 1;
//	routeVector.resize(routeVectorSize);
//	for (int i = 0; i < routeVectorSize; i++)
//	{
//		routeVector[i] = Vector2::Normalize(routeNodeVectors[i + 1] - routeNodeVectors[i]);
//	}
//
//	ReturnHitObjectNode();
//	return true;
//}


#pragma endregion

#pragma region 新経路探索

void MelLib::RouteSearch::SetNodePosition
(
	const Vector3& leftDownNearPos,
	const Vector3& rightUpFarPos,
	const Value3<UINT>& nodeNum,
	std::vector<std::vector<std::vector<AStarNode>>>& nodes
)
{
	// 休憩後にやること
	// 範囲をどっちも0にしたときにもsizeを0じゃなくてちゃんとセットする
	// または、範囲セットしても1だったら座標を0にする


	nodes.clear();
	nodes.resize(nodeNum.v3, std::vector< std::vector<AStarNode>>(nodeNum.v2, std::vector<AStarNode>(nodeNum.v1)));


	for (int z = 0; z < nodes.size(); z++)
	{

		for (int y = 0; y < nodes[z].size(); y++)
		{

			for (int x = 0; x < nodes[z][y].size(); x++)
			{
				//サイズ計算
				nodes[z][y][x].size = (rightUpFarPos - leftDownNearPos).Abs();
				nodes[z][y][x].size.x /= nodeNum.v1;
				nodes[z][y][x].size.y /= nodeNum.v2;
				nodes[z][y][x].size.z /= nodeNum.v3;

				//座標計算
				nodes[z][y][x].position = leftDownNearPos + nodes[z][y][x].size * Vector3(x, y, z);
				if (nodeNum.v1 == 1)nodes[z][y][x].position.x = 0;
				if (nodeNum.v2 == 1)nodes[z][y][x].position.y = 0;
				if (nodeNum.v3 == 1)nodes[z][y][x].position.z = 0;
			}
		}
	}
}

void MelLib::RouteSearch::SetHitObjectFlag
(
	const std::vector<BoxData>& hitObjectsBoxData,
	std::vector<std::vector<std::vector<AStarNode>>>& nodes
)
{
	for (auto& nZ : nodes)
	{
		for (auto& nY : nZ)
		{
			for (auto& nX : nY)
			{
				for (int i = 0; i < hitObjectsBoxData.size(); i++)
				{
					BoxData nodeBoxData;
					nodeBoxData.SetPosition(nX.position);
					nodeBoxData.SetSize(nX.size);

					if (Collision::BoxAndBox(nodeBoxData, nullptr, hitObjectsBoxData[i], nullptr))
					{
						nX.hitObjectNode = true;
						break;
					}
				}
			}
		}
	}

}

void MelLib::RouteSearch::SetCost(const std::vector<BoxData>& hitObjectsBoxData, const std::vector<UINT>& costs, std::vector<std::vector<std::vector<AStarNode>>>& nodes)
{
	if (hitObjectsBoxData.size() != costs.size())return;
	for (auto& nZ : nodes)
	{
		for (auto& nY : nZ)
		{
			for (auto& nX : nY)
			{
				for (int i = 0; i < hitObjectsBoxData.size(); i++)
				{
					BoxData nodeBoxData;
					nodeBoxData.SetPosition(nX.position);
					nodeBoxData.SetSize(nX.size);

					if (Collision::BoxAndBox(nodeBoxData, nullptr, hitObjectsBoxData[i], nullptr))
					{
						nX.cost += costs[i];
					}
				}
			}
		}
	}
}

bool MelLib::RouteSearch::CheckStraightMove(const Vector3& startPos, const Vector3& endPos, std::vector<std::vector<std::vector<AStarNode>>>& nodes, const float straightDis, Vector3& startToEnd)
{
	bool straightMove = true;
	Segment3DData segment;
	segment.SetPosition(MelLib::Value2<MelLib::Vector3>(startPos, endPos));

	for (const auto& nZ : nodes)
	{
		for (const auto& nY : nZ)
		{
			for (const auto& nX : nY)
			{
				BoxData box;
				box.SetPosition(nX.position);
				box.SetSize(nX.size);

				if(nX.position == MelLib::Vector3(0,0,8))
				{
					int z = 0;
				}

				// 当たらなかったら次へ
				if (!Collision::BoxAndSegment3D(box, segment))continue;

				// 進行不可能ノードだったら直進不可なので確認終了
				if (nX.hitObjectNode)
				{
					straightMove = false;
					break;
				}

				// 別のノードと確認
				for (const auto& nZ2 : nodes)
				{
					for (const auto& nY2 : nZ2)
					{
						for (const auto& nX2 : nY2)
						{
							// ヒットオブジェクトじゃなかったら次へ
							if (!nX2.hitObjectNode)continue;

							if ((nX.position - nX2.position).Length() < straightDis)
							{
								straightMove = false;
								break;
							}
						}
						if (!straightMove)break;
					}
					if (!straightMove)break;
				}
				if (!straightMove)break;
			}
			if (!straightMove)break;
		}
		if (!straightMove)break;
	}

	// 可能だったら直進して処理終了
	if (straightMove)
	{
		startToEnd = (endPos - startPos).Normalize();
		return true;
	}
}

bool MelLib::RouteSearch::CalcRoute
(
	const Vector3& startPos,
	const Vector3& endPos,
	std::vector<std::vector<std::vector<AStarNode>>>& nodes,
	std::vector<Vector3>* routeVector,
	std::vector<Vector3>* routeNodePos
)
{

	//スタート地点に一番近いノードの距離を格納する変数
	float startMinDistance = FLT_MAX;
	//ゴール地点に一番近いノードの距離を格納する変数
	float endMinDistance = FLT_MAX;

	auto nodeXArrayNum = nodes[0][0].size();
	auto nodeYArrayNum = nodes[0].size();
	auto nodeZArrayNum = nodes.size();

	//スタートの添え字
	int startNodeIndexX = 0;
	int startNodeIndexY = 0;
	int startNodeIndexZ = 0;
	bool trueStartNodeHitFlag = false;

	//ノードの配列のゴール地点の場所を示す添え字
	int endNodeIndexX = 0;
	int endNodeIndexY = 0;
	int endNodeIndexZ = 0;
	bool trueEndNodeHitFlag = false;


	struct AStarCalcData
	{
		//計算結果(ステップ + 距離 + コスト)
		UINT calcNum = UINT_MAX;

		//配列のインデックス
		int indexX = INT_MAX;
		int indexY = INT_MAX;
		int indexZ = INT_MAX;

		//close配列のインデックス
		int closeIndex = INT_MAX;

		//スタートからの距離
		int stepNum = 0;

		AStarNode* pNode = nullptr;
		//前のノードのデータ
		AStarCalcData* previousNode = nullptr;

		bool openFlag = false;
		bool closeFlag = false;

	};
	std::vector<std::vector<std::vector<AStarCalcData>>>calcData
	(nodeZArrayNum, std::vector<std::vector<AStarCalcData>>(nodeYArrayNum, std::vector<AStarCalcData>(nodeXArrayNum)));

	//ノードセット
	for (int z = 0; z < calcData.size(); z++)
	{

		for (int y = 0; y < calcData[z].size(); y++)
		{
			for (int x = 0; x < calcData[z][y].size(); x++)
			{
				calcData[z][y][x].pNode = &nodes[z][y][x];
			}
		}
	}


	//一時的にfalseにしたときに戻す処理
	auto ReturnHitObjectNode = [&]()
	{
		if (trueStartNodeHitFlag)nodes[startNodeIndexZ][startNodeIndexY][startNodeIndexX].hitObjectNode = true;
		if (trueEndNodeHitFlag)nodes[endNodeIndexZ][endNodeIndexY][endNodeIndexX].hitObjectNode = true;

	};

	for (int z = 0; z < nodeZArrayNum; z++)
	{
		for (int y = 0; y < nodeYArrayNum; y++)
		{
			for (int x = 0; x < nodeXArrayNum; x++)
			{
				float distance = 0.0f;
				distance = LibMath::CalcDistance3D(nodes[z][y][x].position, startPos);

				if (distance <= startMinDistance)
				{
					startMinDistance = distance;
					startNodeIndexX = x;
					startNodeIndexY = y;
					startNodeIndexZ = z;

				}

				distance = LibMath::CalcDistance3D(nodes[z][y][x].position, endPos);
				if (distance <= endMinDistance)
				{
					endMinDistance = distance;
					endNodeIndexX = x;
					endNodeIndexY = y;
					endNodeIndexZ = z;


				}

				//インデックス代入
				calcData[z][y][x].indexX = x;
				calcData[z][y][x].indexY = y;
				calcData[z][y][x].indexZ = z;
			}
		}
	}

	if (nodes[startNodeIndexZ][startNodeIndexY][startNodeIndexX].hitObjectNode)
	{
		//一時的にfalse
		nodes[startNodeIndexZ][startNodeIndexY][startNodeIndexX].hitObjectNode = false;
		trueStartNodeHitFlag = true;
	}

	if (nodes[endNodeIndexZ][endNodeIndexY][endNodeIndexX].hitObjectNode)
	{
		//一時的にfalse
		nodes[endNodeIndexZ][endNodeIndexY][endNodeIndexX].hitObjectNode = false;
		trueEndNodeHitFlag = true;
	}

	//ゴールのノードまでの距離を求めるラムダ式
	auto CalcNodeDistance = []
	(
		const int startX,
		const int startY,
		const int startZ,
		const int endX,
		const int endY,
		const int endZ
		)
	{
		//Xの差
		int startXToEndXDiff = abs(startX - endX);
		//Yの差
		int startYToEndYDiff = abs(startY - endY);
		int startZToEndZDiff = abs(startZ - endZ);

		//Xの差とYの差のどちらが大きいか求める。斜め移動ありの場合、大きいほうが最短距離
		if (startXToEndXDiff > startYToEndYDiff)
		{
			if (startXToEndXDiff > startZToEndZDiff)return startXToEndXDiff;
			else return startZToEndZDiff;
		}
		else
		{
			if (startYToEndYDiff > startZToEndZDiff)return startYToEndYDiff;
			else return startZToEndZDiff;
		}


	};

	//スタートのノードのインデックスを代入
	int startToEndDis = CalcNodeDistance(startNodeIndexX, startNodeIndexY, startNodeIndexZ, endNodeIndexX, endNodeIndexY, endNodeIndexZ);
	calcData[startNodeIndexZ][startNodeIndexY][startNodeIndexX].calcNum =
		startToEndDis + nodes[startNodeIndexZ][startNodeIndexY][startNodeIndexX].cost;

	std::vector<AStarCalcData*>openNodes(1, &calcData[startNodeIndexZ][startNodeIndexY][startNodeIndexX]);
	std::vector<AStarCalcData*>closeNodes;


	AStarCalcData* endNode = nullptr;


	//ステップの計算は周りのノード調べるときにやる?


	while (1)
	{
		//探索終了かどうか
		bool checkEnd = false;

		//ゴールにたどり着けない場合
		if (openNodes.size() == 0)
		{
			ReturnHitObjectNode();
			return false;
		}

		//並び替え
		std::sort
		(
			openNodes.begin(),
			openNodes.end(),
			[]
		(
			const AStarCalcData* node1,
			const AStarCalcData* node2
			)
		{
			int num1 = node1->calcNum;
			int num2 = node2->calcNum;

			return num1 > num2;
		});

		//calcNumが一番少ないやつを取得
		AStarCalcData* mainNode = nullptr;
		int minCalcNum = openNodes[openNodes.size() - 1]->calcNum;
		float minDis = FLT_MAX;
		int mainNodeOpenIndex = 0;
		for (int i = openNodes.size() - 1;; i--)
		{
			//float dis = LibMath::CalcDistance3D(openNodes[i]->pNode->position, nodes[startNodeIndexZ][startNodeIndexY][startNodeIndexX].position);
			float dis = LibMath::CalcDistance3D(openNodes[i]->pNode->position, nodes[endNodeIndexZ][endNodeIndexY][endNodeIndexX].position);
			if (minDis > dis)
			{
				minDis = dis;
				mainNode = openNodes[i];
				mainNodeOpenIndex = i;
			}
			if (openNodes[i]->calcNum != minCalcNum || i == 0)break;
		}

		//オープンに追加するノードを格納する配列
		std::vector<AStarCalcData*>openPushBackNode;

		for (int z = -1; z < 2; z++)
		{
			int indexZ = mainNode->indexZ + z;
			if (indexZ <= -1 || indexZ >= nodeZArrayNum)continue;

			for (int y = -1; y < 2; y++)
			{
				int indexY = mainNode->indexY + y;

				//Y範囲外指定防止
				if (indexY <= -1 || indexY >= nodeYArrayNum)continue;

				for (int x = -1; x < 2; x++)
				{
					int indexX = mainNode->indexX + x;

					//X範囲外指定防止
					if (indexX <= -1 || indexX >= nodeXArrayNum)continue;

					//基準検索防止
					if (indexX == mainNode->indexX && indexY == mainNode->indexY && indexZ == mainNode->indexZ)continue;

					AStarCalcData* checkNode = &calcData[indexZ][indexY][indexX];

					//オブジェクトに重なってたら次のノードへ　
					if (checkNode->pNode->hitObjectNode)
					{
						checkNode->closeFlag = true;
						continue;
					}


					//calcNum = ステップ数 + ゴールまでの距離 + コスト
					int calcNum = mainNode->stepNum + 1
						+ CalcNodeDistance(indexX, indexY, indexZ, endNodeIndexX, endNodeIndexY, endNodeIndexZ)
						+ checkNode->pNode->cost;

					//条件を満たしたら代入
					if (checkNode->openFlag)
					{
						if (calcNum < checkNode->calcNum)
						{
							checkNode->calcNum = calcNum;
							checkNode->previousNode = mainNode;
							checkNode->stepNum = mainNode->stepNum + 1;
						}

						continue;
					}

					//条件を満たしたら、closeからopenに移動&代入
					if (checkNode->closeFlag)
					{
						if (calcNum < checkNode->calcNum)
						{
							//closeから削除
							closeNodes.erase(closeNodes.begin() + checkNode->closeIndex);
							checkNode->closeFlag = false;
							checkNode->closeIndex = INT_MAX;

							//openに追加
							openNodes.push_back(checkNode);
							checkNode->openFlag = true;

							checkNode->calcNum = calcNum;
							checkNode->previousNode = mainNode;
							checkNode->stepNum = mainNode->stepNum + 1;
						}
						continue;
					}


					checkNode->calcNum = calcNum;
					checkNode->previousNode = mainNode;
					checkNode->stepNum = mainNode->stepNum + 1;

					if (checkNode->indexX == endNodeIndexX
						&& checkNode->indexY == endNodeIndexY
						&& checkNode->indexZ == endNodeIndexZ)
					{
						checkEnd = true;
						endNode = checkNode;
					}


					openPushBackNode.push_back(checkNode);
					checkNode->openFlag = true;
				}
			}
		}

		if (checkEnd)break;

		closeNodes.push_back(mainNode);
		mainNode->closeFlag = true;
		mainNode->closeIndex = closeNodes.size() - 1;

		//検索したやつを消す
		openNodes[openNodes.size() - 1]->openFlag = false;
		openNodes.erase(openNodes.begin() + mainNodeOpenIndex);


		for (auto& n : openPushBackNode)
		{
			openNodes.push_back(n);
		}
		openPushBackNode.clear();
	}

	AStarCalcData* currentNode = endNode;
	std::vector<Vector3>routeNodePositions;
	while (1)
	{
		if (!currentNode)break;

		routeNodePositions.push_back(currentNode->pNode->position);

		currentNode = currentNode->previousNode;
	}
	std::reverse(routeNodePositions.begin(), routeNodePositions.end());

	if (routeNodePos)
	{
		routeNodePos->clear();
		*routeNodePos = routeNodePositions;
	}

	//ベクトルを求める
	if (routeVector)
	{
		std::vector<Vector3>& refRouteVector = *routeVector;
		refRouteVector.clear();
		size_t routeVectorSize = routeNodePositions.size() - 1;
		refRouteVector.resize(routeVectorSize);
		for (size_t i = 0; i < routeVectorSize; i++)
		{
			refRouteVector[i] = Vector3::Normalize(routeNodePositions[i + 1] - routeNodePositions[i]);
		}
	}

	ReturnHitObjectNode();
	return true;
}

#pragma endregion

