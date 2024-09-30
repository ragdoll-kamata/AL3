#pragma once
class Scene {
public:
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Scene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Scene();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	bool IsFinished() const { return finished_; }
	bool IsClear() const { return isClear_; }

protected:
	bool finished_;
	bool isClear_;
};
