# DataFlowGraph

This is a data flow graph core implementation.

本プロジェクトはあくまでデータフローグラフのフレームワークに留める。
データフローグラフが所望の機能を満たしたら別プロジェクトを立てて
このデータフローグラフを用いてアプリケーションを構築する。
具体的にはCPUのエミュレーションをターゲットとする。

- 以下のステップで進める。
1. 川上から川下への一方向性を持ったグラフを実行できるようにする。
 1. NOT, AND, ORなど基本的な論理回路を構成できることを確認する。
1. サブファンクション化
   部分グラフをファンクション化し外部からライブラリ的に使用できるようにする。
   サブファンクションの境界はインタフェイス用のノードを配置する必要がある。
1. 逆走するノードを許容するようにする。
 1. フィードバックを許容するために遅延回路(エッジ)を設ける。
    以下、設計上の検討点。
    - 遅延回路をエッジの機能として暗黙に許容できるようにするか
    - 回路設計者が明示的に遅延ノードを挿入する必要があるか
 1. また同時に複数回の実行ができるようにする(でないとフィードバックの意味がない)
    そのためには実行済みフラグ、データ到達フラグのクリアの機構が必要。
    また、遅延回路におけるデータ到達の考え方の整理が必要。

1. 加算回路
1. フリップフロップ 

応用としてのグラフが大規模になってきた場合のために、
容易にグラフを構成できるような仕組みを検討する。
例えば、
- DSLとしてグラフ記述言語を設け、その記述を実行することにより
  対応するC++コードを自動生成
- このDSLをフロントエンドとして実行するとバックエンドとしてデータフローグラフ
  上でのグラフ構築、実行
など。


# Structure / Hierarchy

           Node  Edge
            Executor
    NodeSubSystem GraphBuilder
           NodeComplex
            NodeBase
     NodeLogics  NodeArithmetic



# History
## 2021.01.31

###
開発着手。

###
- ノードとエッジを組合せたグラフ構造を構築できるフレームワークを設ける。
  利用する側ノードとエッジを元に(ノードは継承し、個別のデータ処理を実現する)
  自由にグラフを構成する。グラフ構造に基づき、データが流れていくことにより
  "計算"を実行する。


## 2021.02.10
- 基本的グラフ構造の実行OK。但しまだ、開始ノード、終了ノードの2ノード
  及びその間の2エッジ接続のみ。

- グラフは最も簡単な深さ優先探索でたどる。
  ノードはexecute()が呼ばれると実行済フラグを立てるようにしている。
  これにより複数入力エッジがある場合複数回ノードが実行されることを防ぐ。
  一方、エッジにはデータが揃ったことを示すフラグを設けている。
  ノードの実行を行うか否かは、入力エッジ全てにデータが揃っているか否かで
  判断する。即ち、ノードの実行は入力エッジにデータが全て揃った後の始めの
  実行(execute())時に行われる。

- グラフをたどって、ノードを実行するのはExecutorが行う。

## 2021.02.11
- テストコードのファイル分離
- グラフの基本動作の確認
- NOTゲートの実装及び評価

## 2021.02.14
- AND, ORノードの実装、評価
- 実行履歴のクリア処理
- 複合ノード(サブファンクション)の実装中

## 2021.02.21
- std::shared_ptrを廃止。普通のポインタに置き換え
  Node, Edgeで保持するオブジェクトは現状静的で管理者も明確なので
  shared_ptrを使う必要性が少ないため。
- 複合ノード作成(途中)
  複数回実行がうまくできず。
  Executorの持ち方検討の必要性有り。
  サブシステム毎にExecutorを持つのか、全体で１つのExecutorで賄うのか。

## 2021.02.23
- 複合ノード初版作成
- 複合ノードを用いたNORノードの実装

## 2021.02.24
- 複合ノードを用いた２入力、１出力ノードのテンプレート化
  内部ノードとして２入力１出力ノード＋１入力１出力ノードの組合せパターンのものを
  テンプレート化。これにより、NORの書き直し、及びNANDの実装を行った。

## 2021.02.28
- 算術演算ノード作成

## 2021.03.03
- GraphBuilder作成中
  NodeとEdgeの生成及びそれらを接続してグラフを構築する処理をより簡易に記述出来る
  ようにするためのクラス。

## 2021.03.07
- GraphBuilder作成完了
- NodeSubSystemに対するGraphBuilderによるグラフ生成対応完了
  GraphBuilderにより、生成したものをサブノードすることも
  NodeComplexを継承して内部のグラフを生成するだけでサブノードを構築することも可能。

## 2021.03.14
- GraphBuilderでグラフを生成する際、そのグラフの入力ポートを宣言する
  setInPorts()が分岐ノードに対応していないことが判明。
  分岐ノード対応のため、setInPorts()のインタフェイス変更を検討中。
  可変長引数でPortを複数渡していたのを、Portの複数個のまとまりを複数個渡すように
  する必要がある。C++での記述方法検討中。
  メソッドコール時に引数がコピーされないようにする必要がある。

## 2021.03.20
- NodeComplexで入力分岐に対応
- それに伴い、GraphBuilderとNodeComplexのリファクタリング
  サブノード化関連をGraphBuilderから排除し、NodeComplexに集約

## 2021.03.21
- テスト用ファイルをTESTディレクトリに移動
  それに伴い、.vscode以下の.jsonファイルを修正
- 木構造ではないグラフに対応出来るよう、Executorのノード探索方法の変更
- 変数ノードの作成 - 状態値を保持出来る、常時値を取り出せるノードの作成(途中)

## 2021.03.28
- NodeValue完成。値を保持し、入力値があれば次回ステップでその値を出力する。
- 引数。戻り値の参照化(不要なコピーの削除)
- 旧方式の整理。リファクタリング