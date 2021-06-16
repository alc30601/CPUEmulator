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

## 2021.04.04
- 半加算器
- 全加算器
- 4bit加算器
- 4bit加減算器
- bit-数値変換ノード
- 4 to 16デコーダー(デバッグ中)

## 2021.04.10
- GraphBuilderでノードに名前をつけられるようにした。
  ベースのNodeで名前を保持する。
- 4 to 16デコーダ評価完了

## 2021.04.11
- 複合ノード(ComplexNode)の構成時に出力ポート、入力ポートの定義を行っていたが、廃止
  代わりにEntry,Exitノードを陽に表し、入出力もEntry,Exitノードとのouttoでの接続で設定

## 2021.04.30
- Executorを実行方法に講じてサブクラス化
  そのうちの１つとしてExecutorStableEdgeを設ける。
  これは、回路内のEdgeが安定状態になるまで実行を繰り返すものである。
  ExecutorStableEdgeにより、グラフが循環している場合も実行ができるようになった。
- 上記ExecutorStableEdgeを用いて、SR-FlipFlopを作成。

## 2021.05.02
- NAND型SR-FlipKlop実装、確認完了。
- Executorクラスのサブクラスをファイル分割

## 2021.05.05
- JK FlipFlop (Master-Slave)の動作において、不整合あり。
  従来の方式ではNodeの実行順序によって処理結果が変わる可能性がある。
  具体的にはCK,J,Kを入力とする場合、先にJ,Kのみが更新されていた場合
  CKを抑止に設定するケースでCKにより抑止されるべき処理がCK更新前の値を元に
  演算を行ってしまい、Q,Q-invの値が更新されてしまう。それにより後ほどCKの値を
  演算抑止に更新しても、すでにQ,Q-invは更新されてしまっており、その更新後の値に
  対して演算抑止が働くこととなる。
  この問題に対処するために、先に伝搬可能なデータを全て行き渡させた後に
  データが安定するまで演算を繰り返す、２段方式を試行中。

## 2021.05.09
- JK FlipFlop (Master-Slave)の対応で、最初に、決定可能なEdgeを全て浸透させ、
  次いで、全NodeをEdgeが安定するまで実行する２段階にしようとしたが、現状の仕組みでは
  対応できないことが判明。各SubNodeのExecutorが全て同じExecutorで実行されるようになっているため
  この２段階構成が個々のSubNodeに対しても個別に適用されてしまう。即ち、個々のSubNodeで
  Edgeの値が安定するまで個別に実行されることになる。本来は全体の決定かのうなEdgeに値を全て
  行き渡させた上で全体のEdgeが安定するまで実行することを想定していた。
  以上より、対策の方向性を変えることとする。対策案は２つ。
  1) 現状のSubNodeを持つ階層構造を維持したまま、実行ごとにExecutorを動的に切り替えられるようにする。
     これにより、最初の段階では全てのNode、SubNodeに対して、Edgeに決定可能な値を浸透させ、
     次のステップでEdgeが安定させる。このため、現状は個々のSubNodeでExecutorを保持しているが、
     Executorを実行時に指定するように変更する必要がある。
  2) 全てのSubNodeの改装を平坦化しSubNodeを構造として含まないようにする。
     これにより現状のExecutorの仕組みで上記の2段階実行も可能となる。
     ただしNode構築時に平坦化すると、グラフ構成時の手間が膨大となるため、グラフ構成時は従来同様
     SubNodeを利用した階層的な構成でグラフを構築できるようにする。全体のグラフが構築できた段階で
     平坦化の処理を実行し、SubNodeを解消する。
  「回路」を考えた場合、1)の方式では個々のSubNodeごとにEdgeの値が安定するステップを実行することになるが
  この場合、あるNodeが安定化したあとでそのNodeに入力させる値が変化する可能性がある。その場合再度
  Node内のEdgeの安定化処理を実行する必要がある。(現状もそうだが)
  よって、「回路」を想定した場合、平坦化し全てを１つの大きなグラフとして階層無しに取り扱ったほうが
  よいと思われる。よって、2)の方式で進めることとする。

## 2021.05.16
- 前述のようにグラフの平坦化を実装。
  しかしJK FlipFlopでEdge状態安定せず。(以前の版では安定していたパターンも)
  解析が必要。
  不安定となるパターンを抽出し、原因の特定を行う必要がある。
  JK FlipFlopに関しては3パターンを繰り返していた。全Edge中、半数ほどが安定していない。

## 2021.05.23
- 上記の問題解析のため、NANDのNodeを設ける。AND,NOTで構成していたNANDをプリミティブなNodeとして構成。
  JK-FFは正常に動作するようになった。何故、AND,NOTの合成で駄目でプリミティブなNANDでうまく行くのかは未解明。
  他もいくつか触っているのでその影響の可能性もある。
  しかし、JK FF(Master-Slave)はやはり正解とならない。未解析。

## 2021.06.06
- 合成でないNANDを用いることで少し前進。
  JK FlipFlop(Matste-Slave)ではトリガー前の不定状態は評価対象外とすることで
  想定通りの動作にすることができた。
- 一方、NANDだとOKだが、ANDとNOTの合成のNANDだとやはりうまく行かない。
  JK FlipFlopで状態が安定せず無限ループする。
- グラフ平坦化で、Entry,Exitノードを除去するようにしてグラグを単純化するつもり。

## 2021.06.16
- グラフ平坦化のEntry,Exitノード除去版の実装(検証未)
