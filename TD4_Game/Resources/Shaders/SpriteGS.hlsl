#include "Sprite.hlsli"

[maxvertexcount(4)]
void main(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	//出力用変数
    GSOutput element;
	
	//縦横幅をもとに出力オフセットを作成
    float2 sizeOffset[4] =
    {
        { 0              , input[0].size.y },	//左下
        { 0              , 0 },					//左上
        { input[0].size.x, input[0].size.y },					//右下
        { input[0].size.x, 0 },	//右上
    };
	//uvオフセットを格納
	float2 uvOffset[4] =
	{
        { input[0].uvOffset.x, input[0].uvOffset.w },
        { input[0].uvOffset.x, input[0].uvOffset.y },
        { input[0].uvOffset.z, input[0].uvOffset.w },
        { input[0].uvOffset.z, input[0].uvOffset.y },
    };
	
	//点の入力から四角形を出力
	for (uint i = 0; i < 4; i++)
	{
        element.pos = input[0].svpos;
		//出力頂点のローカル座標を、描画サイズに調整（左上基準）
        element.pos.xy += sizeOffset[i];
		//ローカル座標をワールド座標に変換
        element.pos = mul(input[0].ins_matrix, element.pos);
		//uv座標はオフセットから（あとで外部から取得）
		element.uv = uvOffset[i];
		//ほかは使わんのでそのまま
        element.ins_matrix = input[0].ins_matrix;
        element.size = input[0].size;
        element.uvOffset = input[0].uvOffset;
        element.color = input[0].color;
        element.freedata = input[0].freedata;
		//出力
		output.Append(element);
	}
}