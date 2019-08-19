[component::Component](https://github.com/ekazyenereta/Component/blob/master/Component.h)<BR>

	`
	namespace component {
		class Component;
	}
	`

# 概要
Componentは、Component を継承した機能を追加、親子付けする機能である。
### 管理
Componentの管理には [std::shared_ptr](https://cpprefjp.github.io/reference/memory/shared_ptr.html) を利用しています。
### 参照
Componentの参照には [IReference](https://github.com/ekazyenereta/Reference/wiki/IReference) を利用しています。

# メンバ関数
| 名前 | 説明 |
| ------ | ------ |
| (constructor) | コンストラクタ |
| (destructor) | デストラクタ | 
| ThisComponent | 自身の参照機能を取得する | 
| AddComponent | コンポーネントを追加する | 
| GetComponent | コンポーネントの取得する | 
| SetComponent | コンポーネントを登録する | 
| DestroyComponent | コンポーネントを破棄する | 
| SetComponentName | コンポーネントの名前を設定する |
| GetComponentName | コンポーネントの名前を取得する |
| GetNumChild | 登録済みコンポーネント、子要素の数を取得する |
| GetComponentChild | 子要素を取得する |
| GetComponentParent | 親要素を取得する |

# 例
### Componentの基本的な使い方
[Source.cpp](https://github.com/ekazyenereta/Component/blob/master/Source.cpp)<BR>
