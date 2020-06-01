#include "VS_HS_transfer.hlsli"
#include "HS_DS_transfer.hlsli"




float3 transformPosToBezierTri(float3 p, float3 cp /* nearest cp */, float3 cpn /* nearest cps normal */)
{
    float3 v = p - cp;
    float len = dot(cpn, v);
    float3 dir = len * cpn;
    return p - dir;
}


PatchTess ConstFun(InputPatch<HS_IN, 3> cps, uint patchId : SV_PrimitiveID)
{
    PatchTess pt;
    pt.EdgeTess[0] = 16;
    pt.EdgeTess[1] = 16;
    pt.EdgeTess[2] = 16;
    pt.InsideTess[0] = 16;
    
    
    
    pt.B003 = cps[0].worldPos;
    pt.N003 = normalize(cps[0].normal);
    pt.B030 = cps[1].worldPos;
    pt.N030 = normalize(cps[1].normal);
    pt.B300 = cps[2].worldPos;
    pt.N300 = normalize(cps[2].normal);
    
    float3 edgeB300 = pt.B030 - pt.B003;
    float3 edgeB030 = pt.B003 - pt.B300;
    float3 edgeB003 = pt.B300 - pt.B030;
    
    // original triangle trisection points
    float3 B012 = pt.B003 + edgeB300 * (1.0 / 3.0);
    float3 B021 = pt.B003 + edgeB300 * (2.0 / 3.0);
    
    float3 B120 = pt.B030 + edgeB003 * (1.0 / 3.0);
    float3 B210 = pt.B030 + edgeB003 * (2.0 / 3.0);
    
    float3 B102 = pt.B300 + edgeB030 * (1.0 / 3.0);
    float3 B201 = pt.B300 + edgeB030 * (2.0 / 3.0);
    
    // transform trisection point to bezier triangle trisections
    pt.B012 = transformPosToBezierTri(pt.B003, B012, pt.N003 );
    pt.B021 = transformPosToBezierTri(pt.B030, B021, pt.N030 );   
    pt.B120 = transformPosToBezierTri(pt.B030, B120, pt.N030 );
    pt.B210 = transformPosToBezierTri(pt.B300, B210, pt.N300 );   
    pt.B102 = transformPosToBezierTri(pt.B003, B102, pt.N003 );
    pt.B201 = transformPosToBezierTri(pt.B300, B201, pt.N300 );
    
    // calculating the flat triangle's centroid
    float3 triCenter = (pt.B003 + pt.B030 + pt.B300) / 3.0;
    // centroid of bezier tri should be the average of the trisection
    // point and a little bit of additional height on that direction
    pt.B111 = (pt.B012 + pt.B021 + pt.B120 + pt.B210 + pt.B201 + pt.B102) / 6.0;
    pt.B111 = pt.B111 + (pt.B111 - triCenter) / 2.0;
    
   //pt.B012 =  B012;
   //pt.B021 =  B021;
   //pt.B120 =  B120;
   //pt.B210 =  B210;
   //pt.B102 =  B102;
   //pt.B201 =  B201;
   //pt.B111 = triCenter;
    
    return pt;
}


[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstFun")]
[maxtessfactor(64)]
HS_OUT main(InputPatch<HS_IN, 3> cp, uint i : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    HS_OUT hso;
    
    // It is problematic to generate the 10 control points right
    // here. It would require switch-casing on i.
    hso.worldPos = cp[i].worldPos;
    hso.normal = normalize(cp[i].normal);
    hso.tex = cp[i].tex;    
    
    return hso;
}


