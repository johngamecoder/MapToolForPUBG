//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// TextureMapping
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string TextureMapping_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjMatrix : Projection;

float gConst;
float gCos;
float4 gOutlineColor;

texture DiffuseMap_Tex;
sampler2D DiffuseSampler = sampler_state { Texture = (DiffuseMap_Tex); };

struct VS_INPUT 
{
   float4 mPosition : POSITION;
   float3 mNormal : NORMAL;
   float2 mTexCoord : TEXCOORD0;
   
};

struct VS_OUTPUT 
{
   float4 mPosition : POSITION;
   float2 mTexCoord : TEXCOORD0;
};

struct PS_INPUT
{
    float2 mTexCoord : TEXCOORD0;
};


VS_OUTPUT VS_OUTLINE( VS_INPUT Input )
{
    VS_OUTPUT Output;
    Input.mNormal = normalize(Input.mNormal);
    Output.mPosition = float4(Input.mPosition.xyz + (Input.mNormal * gConst), 1);
    Output.mPosition = mul(Output.mPosition, gWorldMatrix);
    Output.mPosition = mul(Output.mPosition, gViewMatrix);
    Output.mPosition = mul(Output.mPosition, gProjMatrix);
   
    Output.mTexCoord = Input.mTexCoord;
    return Output;
}

float4 PS_OUTLINE(PS_INPUT Input) : COLOR
{
    return gOutlineColor;
}


VS_OUTPUT VS1(VS_INPUT Input)
{
    VS_OUTPUT Output;
    Output.mPosition = mul(Input.mPosition, gWorldMatrix);
    Output.mPosition = mul(Output.mPosition, gViewMatrix);
    Output.mPosition = mul(Output.mPosition, gProjMatrix);
    
    Input.mTexCoord.y = Input.mTexCoord.y + (Input.mPosition.x) / 200.0f;
    Output.mTexCoord = Input.mTexCoord;
    return Output;
}

float4 PS1(PS_INPUT Input) : COLOR
{
    float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);
    return albedo.rgba;
}
//--------------------------------------------------------------//
// Technique Section for TextureMapping
//--------------------------------------------------------------//
technique TextureMapping
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 VS_OUTLINE();
        PixelShader = compile ps_2_0 PS_OUTLINE();
    }
    pass Pass_1
    {
        VertexShader = compile vs_2_0 VS1();
        PixelShader = compile ps_2_0 PS1();
    }
}

