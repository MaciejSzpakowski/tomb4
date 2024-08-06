#include "../tomb4/pch.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "../global/types.h"
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

const char rc_PixelShader[] = R"(
Texture2D textures[1];
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Col : COLOR;
	float2 TexCoord : TEXCOORD;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    float4 result = textures[0].Sample(ObjSamplerState, input.TexCoord);
    clip(result.a - 1.0f);
	return result;
}
)";

const char rc_VertexShader[] = R"(

struct vertex
{
    float4 pos;
    float4 light;
    float4 tex;
};

cbuffer jedziemy
{
	vertex inputData[1024];
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Col : COLOR;
	float2 TexCoord : TEXCOORD;
};

VS_OUTPUT main(uint vid : SV_VertexID)
{
	VS_OUTPUT output;
	output.Pos = float4(inputData[vid].pos.x,inputData[vid].pos.y,inputData[vid].pos.z,1);
	output.Col = float4(0, 0, 0, 0);
    output.TexCoord = float2(inputData[vid].tex.x,inputData[vid].tex.y);
	return output;
}
)";

struct renderer
{
    IDXGISwapChain* swapChain;
    ID3D11RenderTargetView* backBuffer;
    ID3D11Device* device;
    ID3D11DeviceContext* context;
    ID3D11VertexShader* defaultVS;
    ID3D11PixelShader* defaultPS;
    ID3D11PixelShader* defaultPost;
    ID3D11DepthStencilView* depthStencilView;
    ID3D11Texture2D* depthStencilBuffer;
    ID3D11RasterizerState* wireframe;
    ID3D11RasterizerState* solid;
    ID3D11SamplerState* point;
    ID3D11Buffer* cbufferVS;
    ID3D11Buffer* cbufferVScamera;
    float w, h;

    ID3D11ShaderResourceView* tex[200];
};

renderer r;

void checkhr(HRESULT hr, int line)
{
    if (hr == 0) return;
    char str[128];
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0,
        hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
        str, 128, 0);
    MessageBox(0, str, 0, 0);
}

ID3D11ShaderResourceView* createTextureFromBytes(void* data, UINT width, UINT height)
{
    ID3D11Texture2D* tex = nullptr;
    D3D11_TEXTURE2D_DESC desc;
    D3D11_SUBRESOURCE_DATA sub;

    sub.pSysMem = (void*)data;
    sub.SysMemPitch = (UINT)width * 4;
    sub.SysMemSlicePitch = (UINT)height * (UINT)width * 4;

    desc.Width = (UINT)width;
    desc.Height = (UINT)height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;

    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    HRESULT hr = r.device->CreateTexture2D(&desc, &sub, &tex);
    checkhr(hr, __LINE__);

    D3D11_TEXTURE2D_DESC desc2;
    tex->GetDesc(&desc2);
    ID3D11ShaderResourceView* srv = nullptr;
    hr = r.device->CreateShaderResourceView(tex, 0, &srv);
    checkhr(hr, __LINE__);
    tex->Release();

    return srv;
}

ID3D11SamplerState* createSampler()
{
    ID3D11SamplerState* sampler;
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    r.device->CreateSamplerState(&sampDesc, &sampler);
    return sampler;
}

ID3D11PixelShader* createPixelShaderFromString(const char* str,
    const char* entryPoint, const char* target)
{
    ID3D11PixelShader* result;
    ID3D10Blob* ps;
    ID3D10Blob* errorMsg;
    HRESULT hr = D3DCompile(str, strlen(str), 0, 0, 0, entryPoint, target, 0, 0, &ps, &errorMsg);
    if (errorMsg != nullptr)
    {
        void* ptr = errorMsg->GetBufferPointer();
        SIZE_T sz = errorMsg->GetBufferSize();
        byte buffer[1000];
        memset(buffer, 0, 1000);
        memcpy(buffer, ptr, sz);
        MessageBox(0, (LPCSTR)buffer, 0, 0);
        return nullptr;
    }
    else
    {
        checkhr(hr, __LINE__);
    }
    //D3DCompile
    hr = r.device->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), 0, &result);
    checkhr(hr, __LINE__);
    ps->Release();
    return result;
}

ID3D11VertexShader* createVertexShaderFromString(const char* str,
    const char* entryPoint, const char* target)
{
    ID3D11VertexShader* result = nullptr;
    ID3D10Blob* vs;
    ID3D10Blob* errorMsg;
    HRESULT hr = D3DCompile(str, strlen(str), 0, 0, 0, entryPoint, target, 0, 0, &vs, &errorMsg);

    if (errorMsg != nullptr)
    {
        void* ptr = errorMsg->GetBufferPointer();
        SIZE_T sz = errorMsg->GetBufferSize();
        byte buffer[1000];
        memset(buffer, 0, 1000);
        memcpy(buffer, ptr, sz);
        MessageBox(0, (LPCSTR)buffer, 0, 0);
        return nullptr;
    }
    else
    {
        checkhr(hr, __LINE__);
    }
    hr = r.device->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), 0, &result);
    checkhr(hr, __LINE__);
    vs->Release();
    return result;
}

int d3d11(HWND hwnd, UINT w, UINT h)
{
    HRESULT hr = 0;
    r.w = w;
    r.h = h;
    memset(r.tex, 0, sizeof(ID3D11ShaderResourceView*) * 200);

    //// *********** PIPELINE SETUP STARTS HERE *********** ////
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = hwnd;                   // the window to be used
    scd.SampleDesc.Quality = 0;
    scd.SampleDesc.Count = 1;                               // no anti aliasing
    scd.Windowed = TRUE;                                    // windowed/full-screen mode
    //scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;   // alternative fullscreen mode

    UINT creationFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;

    ////    DEVICE, DEVICE CONTEXT AND SWAP CHAIN    ////
    hr = D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
        D3D11_SDK_VERSION, &scd, &r.swapChain, &r.device, NULL,
        &r.context);
    checkhr(hr, __LINE__);

    ////    BACK BUFFER AS RENDER TARGET, DEPTH STENCIL   ////
    // get the address of the back buffer
    ID3D11Texture2D* buf;
    r.swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&buf);
    // use the back buffer address to create the render target
    hr = r.device->CreateRenderTargetView(buf, NULL, &r.backBuffer);
    checkhr(hr, __LINE__);
    buf->Release();

    //Describe our Depth/Stencil Buffer and View
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = w;
    depthStencilDesc.Height = h;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    hr = r.device->CreateTexture2D(&depthStencilDesc, NULL, &r.depthStencilBuffer);
    checkhr(hr, __LINE__);
    hr = r.device->CreateDepthStencilView(r.depthStencilBuffer, NULL, &r.depthStencilView);
    checkhr(hr, __LINE__);

    ////   VIEWPORT    ////
    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (FLOAT)w;
    viewport.Height = (FLOAT)h;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    r.context->RSSetViewports(1, &viewport);

    ////    VS and PS    ////
    r.defaultVS = createVertexShaderFromString(rc_VertexShader, "main", "vs_5_0");
    r.context->VSSetShader(r.defaultVS, 0, 0);
    r.defaultPS = createPixelShaderFromString(rc_PixelShader, "main", "ps_5_0");

    // vertex buffer for single instace i.e. UpdateSubresource
#ifdef VI_VALIDATE
    if (sizeof(sprite) % 16 != 0)
    {
        fprintf(stderr, "Sprite is not multiple of 16 bytes");
        return;
    }
#endif
    D3D11_BUFFER_DESC cbbd;
    ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
    cbbd.Usage = D3D11_USAGE_DEFAULT;
    cbbd.ByteWidth = 1024 * 40;
    cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    r.device->CreateBuffer(&cbbd, NULL, &r.cbufferVS);
    r.context->VSSetConstantBuffers(0, 1, &r.cbufferVS);

    // vertex buffer for camera also UpdateSubresource
    ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
    cbbd.Usage = D3D11_USAGE_DEFAULT;
    cbbd.ByteWidth = 16;
    cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    r.device->CreateBuffer(&cbbd, NULL, &r.cbufferVScamera);
    r.context->VSSetConstantBuffers(1, 1, &r.cbufferVScamera);

    D3D11_RASTERIZER_DESC rd;
    ZeroMemory(&rd, sizeof(rd));
    rd.FillMode = D3D11_FILL_WIREFRAME;
    rd.CullMode = D3D11_CULL_NONE;
    hr = r.device->CreateRasterizerState(&rd, &r.wireframe);
    checkhr(hr, __LINE__);
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_BACK;
    hr = r.device->CreateRasterizerState(&rd, &r.solid);
    checkhr(hr, __LINE__);

    r.context->OMSetRenderTargets(1, &r.backBuffer, r.depthStencilView);
    r.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    r.context->RSSetState(r.solid);
    r.context->PSSetShader(r.defaultPS, 0, 0);

    r.point = createSampler();
    r.context->PSSetSamplers(0, 1, &r.point);

    //// *********** PIPELINE SETUP ENDS HERE *********** ////

    return 0;
}

_declspec(align(16))
struct vert
{
    float x, y, z, w;
    float l[4];
    float u,v,a,b;
};
vert gv[1000] = {
    {-1,-1,0,-1},
    {-0.5f,0.75f,0,1},
    {0.5f,-0.5f,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
};

void draw(D3DTLBUMPVERTEX* v, int n)
{
    for (int i = 0; i < n; i++)
    {
        gv[i].x = v[i].sx / 950.0f - 1.0f;
        gv[i].y = -(v[i].sy / 600.0f - 1.0f);
        gv[i].z = v[i].sz;
        gv[i].u = v[i].tu;
        gv[i].v = v[i].tv;
        gv[i].a = v[i].tx;
        gv[i].b = v[i].ty;
    }

    r.context->UpdateSubresource(r.cbufferVS, 0, NULL, gv, 0, 0);
    r.context->Draw(n, 0);
}

void renderStart()
{
    float bg[] = { 1, 1,1,1 };
    r.context->ClearRenderTargetView(r.backBuffer, bg);
    r.context->ClearDepthStencilView(r.depthStencilView,
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void renderEnd()
{
    r.swapChain->Present(0, 0);
}

void addTexture(UINT w, UINT h, void* data, UINT n)
{
    r.tex[n] = createTextureFromBytes(data, w, h);
}

void setTexture(UINT n)
{
    if(r.tex[n])
    r.context->PSSetShaderResources(0, 1, &r.tex[n]);
}