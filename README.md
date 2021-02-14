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


