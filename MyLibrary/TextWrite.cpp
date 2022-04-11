#include "TextWrite.h"
#include"Library.h"
#include"LibWinAPI.h"

using namespace MelLib;

ComPtr<IDWriteFactory> TextWrite::dWriteFactory;
std::unordered_map<std::string, ComPtr<IDWriteTextFormat>>TextWrite::pTextFormat;

ComPtr<ID3D11Device> TextWrite::d3d11Device;
ComPtr<ID3D11DeviceContext>TextWrite::d3d11context;
ComPtr<ID3D11On12Device>TextWrite::d3d11On12device;
ComPtr<ID3D11Resource> TextWrite::wrappedBackBuffer[2];

ComPtr<ID2D1Factory3> TextWrite::d2dFactory;
ComPtr<ID2D1Device2>TextWrite::d2dDevice;
ComPtr<IDXGIDevice>TextWrite::dxgiDevice;
ComPtr<ID2D1DeviceContext> TextWrite::d2dContext;
ComPtr<ID2D1Bitmap1>TextWrite::d2dRenderTerget[2];

ComPtr<ID2D1HwndRenderTarget> TextWrite::d2dRenderTarget;
ComPtr<ID2D1SolidColorBrush> TextWrite::d2dSolidColorBrush;
HWND TextWrite::hwnd;

std::vector<std::wstring> TextWrite::tests;
std::vector<std::string> TextWrite::fontNames;
std::vector<std::tuple<Vector2, Color,/* TextWrite::TextAlignment,*/std::wstring, std::string>> TextWrite::drawTextDatas;

LRESULT TextWrite::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    /* switch (msg)
     {
     case WM_DESTROY:
         PostQuitMessage(0);
         return 0;
     }*/
    return DefWindowProc(hwnd, msg, wparam, lparam);

}


bool TextWrite::Initialize
(
    ID3D12Device* pD3D12Device,
    ID3D12CommandQueue** pPD3D12Queue,
    ID3D12Resource* pBuckBuffers[2]
)
{


    UINT flag =
        D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    auto result = D3D11On12CreateDevice
    (
        pD3D12Device,
        flag,
        nullptr,//nullptrにするとD3D12の方と同じレベルになる
        0,//上の引数の配列数(今回はここで指定しないから0)
        reinterpret_cast<IUnknown**>(pPD3D12Queue),//D3D12のキューのポインタをキャストしたものでいい?
        1,//キューの数
        0,//D3D12デバイスのノード
        &d3d11Device,
        &d3d11context,
        nullptr//選ばれたレベルを取得する場合ポインタを渡す
    );

    //これ調べる
    //https://docs.microsoft.com/ja-jp/windows/win32/api/unknwn/nf-unknwn-iunknown-queryinterface(q)
    //https://docs.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-queryinterface(refiid_void)
    //チュートリアルには「ID3D11Device の作成後、そこから ID3D11On12Device インターフェイスを照会できます。
    //これは、D2D を設定するために使用されるプライマリ デバイス オブジェクトです。」
    //と書かれている
    //引数に渡したやつがnullptrじゃなくなるから、
    //Asを呼んだID3Dオブジェクトと同じ設定でCreateするってこと?
    result = d3d11Device.As(&d3d11On12device);

    result = D2D1CreateFactory
    (
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        IID_PPV_ARGS(&d2dFactory)
    );

    result = d3d11On12device.As(&dxgiDevice);

    //D3D11_CREATE_DEVICE_DEBUGのせいで生成できてなかった
    result = d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice);

    result = d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dContext);

    IDWriteFactory** pPWriteFactory = &dWriteFactory;
    result = DWriteCreateFactory
    (
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(pPWriteFactory)
    );  


    //1インチ(2.54cm)あたりのドット数の取得
    float dpiX, dpiY;
    d2dFactory->GetDesktopDpi(&dpiX, &dpiY);

    //ビットマップオプションろカラーコンテキスト情報の設定?
    D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1
    (
        D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,//ビットマップを生成するためのDXGI形式とアルファモード
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
        dpiX,
        dpiY
    );

    //バックバッファ分ループ
    for (int i = 0; i < 2; i++)
    {
        D3D11_RESOURCE_FLAGS d3d11Flags = {};
        //D3D12のリソースを、D3D11が理解できるようにするためのもの(今回の場合はバックバッファだからD3D11_BIND_RENDER_TARGETを指定?)
        d3d11Flags.BindFlags = D3D11_BIND_RENDER_TARGET;

        //12のバックバッファをもとに11のバックバッファを生成?
        //「このメソッドは、D3D11on12で使用するD3D11リソースを作成します。」ドキュメントより
        //「レンダー ターゲット状態を "IN" 状態とし、表示状態を "OUT" 状態として指定」チュートリアルより
        //ここに書かれているD3D12_RESOURCE_STATEを自動で反映させてくれる?(戻す命令あるとエラー出る)
        //GPUデバッガにて文字描画命令の後に戻してる命令があった。自動で戻してるっぽい。
        //GPUデバッガでは、D3D12_RESOURCE_STATE_PRESENTではなくD3D12_RESOURCE_STATE_COMMONと書かれる
        //DirectX11ではリソースバリアが明示的ではない?内部で動く?
        //https://hexadrive.jp/hexablog/program/13072/　
        result = d3d11On12device->CreateWrappedResource
        (
            pBuckBuffers[i],
            &d3d11Flags,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT,
            IID_PPV_ARGS(&wrappedBackBuffer[i])
        );

        //「このバックバッファに直接描画するD2Dのレンダーターゲットを作成します。」チュートリアルの文
        //IDXGISurfaceインターフェイスは、画像データオブジェクトのメソッドを実装します。(ドキュメントの説明を翻訳した文)
        ComPtr<IDXGISurface>dxgiSurface;
        result = wrappedBackBuffer[i].As(&dxgiSurface);
        result = d2dContext->CreateBitmapFromDxgiSurface
        (
            dxgiSurface.Get(),
            &bitmapProperties,
            &d2dRenderTerget[i]
        );

        //Offsetは、ポインタをずらすものっぽい(ヘッダファイルみたらそんな感じだった)
        //(ずらす回数,ずらすサイズ)
        //ptr += 引数1 * 引数2
        //チュートリアルでは、ハンドルを配列に格納してないので、ずらさないと2つ目のRTVビューを作れないからOffset関数を使っていた
        //そもそも自分の場合ここでビュー作らないからハンドル受け取る必要ない
        //backBufferHandle[i].Offset(1, pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
    }

    //途中で色変えられるっぽい
    result = d2dContext->CreateSolidColorBrush
    (
        D2D1::ColorF(0, 0, 0, 1), 
        &d2dSolidColorBrush
    );


    //result = d2dFactory->CreateHwndRenderTarget
    //(
    //    D2D1::RenderTargetProperties(),
    //    D2D1::HwndRenderTargetProperties(Library::GetHWND(), D2D1::SizeU(Library::GetWindowWidth(), Library::GetWindowHeight())),
    //    &d2dRenderTarget
    //);

    //result = d2dContext->CreateSolidColorBrush
    //(
    //    D2D1::ColorF(D2D1::ColorF::Black),
    //    &d2dSolidColorBrush
    //);





    return true;
}

void TextWrite::LoopStartProcess()
{
    drawTextDatas.clear();
}

void TextWrite::LoopEndProcess(const UINT rtIndex)
{
    //これ書くとリソースバリア切り替えられなくてエラー出る
    //コマンドリストをCloseにした後に呼び出さないといけないのでテキスト描画しない場合に自分で治すことも不可
    //テキスト描画しなくても下にある一連の処理をしないといけない
    //if (drawTextDatas.size() == 0)return;

    //D3D11on12で使用するD3D11リソース(バックバッファ)をセット(d3d11On12deviceにセット)
    d3d11On12device->AcquireWrappedResources(wrappedBackBuffer[rtIndex].GetAddressOf(), 1);

    //レンダーターゲットセット
    d2dContext->SetTarget(d2dRenderTerget[rtIndex].Get());
    d2dContext->BeginDraw();
    d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());

    //描画
    for (const auto& d : drawTextDatas)
    {
        
        Color color = std::get<1>(d);
        d2dSolidColorBrush->SetColor(D2D1::ColorF(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.f));


        Vector2 pos = std::get<0>(d);
        IDWriteTextFormat* pFormat = pTextFormat[std::get<3>(d)].Get();
       /* DWRITE_TEXT_ALIGNMENT alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER;
        switch (std::get<2>(d))
        {
        case TextAlignment::LEFT_JUSTIFIED:
            alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
            break;
        case  TextAlignment::RIGHT_JUSTIFIED:
            alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_TRAILING;
            break;
        case TextAlignment::POINT:


            alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER;
            break;
        }
        pFormat->SetTextAlignment(alignment);*/

        D2D1_RECT_F layoutRect = D2D1::RectF(pos.x, pos.y, Library::GetWindowWidth(), Library::GetWindowHeight());

        const std::wstring& text = std::get<2>(d);
        
        if(!pFormat)
        {
            std::wstring outputWStr = L"テキスト「";
            outputWStr += text.c_str();
            outputWStr += L"」の描画に失敗しました。存在していないFontDataを指定している可能性があります。\n";
            OutputDebugStringW(outputWStr.c_str());

            std::string outputStr = "FontData名「";
            outputStr += std::get<3>(d);
            outputStr += "」\n";
            OutputDebugStringA(outputStr.c_str());

            continue;
        }

        d2dContext->DrawTextW
        (
            text.c_str(),
            text.size(),
            pFormat,
            layoutRect,
            d2dSolidColorBrush.Get()
        );

        
    }

    auto result = d2dContext->EndDraw();

    //D3D11on12用にラップされたD3D11リソースの解放
    //「ReleaseWrappedResources を呼び出すことで、
    //作成時に指定した "OUT" 状態に指定のリソースを移行させるリソース バリアを背後で発生させます。」チュートリアルより
    //「Flushを呼び出す前にこのメソッドを呼び出して、
    //リソースバリアを適切な「アウト」状態に挿入し、リソースバリアが「イン」状態であると予想されることをマークします。」
    //0含め16フレーム目にめちゃくちゃメモリ持ってかれる。なぜか調べる
    //制限事項にかなりのメモリのオーバーヘッドがあると書いてあったからそれ?
    d3d11On12device->ReleaseWrappedResources(wrappedBackBuffer[rtIndex].GetAddressOf(), 1);

    //D3D12のコマンドキューに送信
    //「 最後に、11On12 デバイスで実行されるすべてのコマンドを共有 ID3D12CommandQueue に送信するために、
    //ID3D11DeviceContext で Flush を呼び出す必要があります。」チュートリアルより
    d3d11context->Flush();
}


bool TextWrite::CreateFontData(const std::wstring& fontName,const float size,const std::string& name)
{
    ComPtr<IDWriteTextFormat>pWriteTextFormat;

    //https://docs.microsoft.com/ja-jp/windows/win32/api/dwrite/nf-dwrite-idwritefactory-createtextformat
  
    auto result = dWriteFactory->CreateTextFormat
    (
        fontName.c_str(),//フォントファミリーの名前を含む文字列
        NULL,//フォントコレクションオブジェクトへのポインタ
        DWRITE_FONT_WEIGHT_REGULAR,//太さ(enumで指定)
        DWRITE_FONT_STYLE_NORMAL,//スタイル(enumで指定)。ここで文字を斜めにしたりできる
        DWRITE_FONT_STRETCH_NORMAL,//ストレッチ(enumで指定)。文字の引き伸ばし具合
        size,//ピクセルが単位のフォントサイズ?
        L"en_us",//ロケール名を含む文字列 ロケールとは、言語の書式ルール
        &pWriteTextFormat
    );

    // フォントがなかったりして生成失敗したら、Arialで生成
    if(result != S_OK)
    {
        result = dWriteFactory->CreateTextFormat
        (
            L"Arial",//フォントファミリーの名前を含む文字列
            NULL,//フォントコレクションオブジェクトへのポインタ
            DWRITE_FONT_WEIGHT_REGULAR,//太さ(enumで指定)
            DWRITE_FONT_STYLE_NORMAL,//スタイル(enumで指定)。ここで文字を斜めにしたりできる
            DWRITE_FONT_STRETCH_NORMAL,//ストレッチ(enumで指定)。文字の引き伸ばし具合
            size,//ピクセルが単位のフォントサイズ?
            L"en_us",//ロケール名を含む文字列 ロケールとは、言語の書式ルール
            &pWriteTextFormat
        );
    }

    //文字の揃え方設定。この場合、中央揃え
    //左右位置
    result = pWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    //上下位置
    result = pWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);


    pTextFormat.emplace(name, pWriteTextFormat);
    return true;

}

void TextWrite::Draw
(
    const Vector2& position, 
    const Color& color,
    /*const TextAlignment allgnment,*/
    const std::wstring& text, 
    const std::string& fontName
)
{
    drawTextDatas.push_back(std::make_tuple(position,color,/* allgnment,*/ text, fontName));
}
