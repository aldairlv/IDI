#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

out vec4 FragColor;

uniform vec3 posFocusE;
uniform vec3 colorFocusE;

uniform vec3 posFocusID0;
uniform vec3 colorFocusID0;

uniform vec3 posFocusID1;
uniform vec3 colorFocusID1;

uniform vec3 posFocusID2;
uniform vec3 colorFocusID2;

uniform vec3 posFocusID3;
uniform vec3 colorFocusID3;

uniform mat4 TGfocus;

vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
    vec3 posFocusEe = (View * vec4(posFocusE,1.0)).xyz;
    vec3 N = normalize(fnormal);
    vec3 LE = normalize(posFocusEe - fvertex);
    
    vec3 posFocusID0F = (View * TGfocus * vec4(posFocusID0, 1.0)).xyz;
    vec3 posFocusID1F = (View * TGfocus * vec4(posFocusID1, 1.0)).xyz;
    vec3 posFocusID2F = (View * TGfocus * vec4(posFocusID2, 1.0)).xyz;
    vec3 posFocusID3F = (View * TGfocus * vec4(posFocusID3, 1.0)).xyz;
    
    vec3 LF0 = normalize(posFocusID0F - fvertex);
    vec3 LF1 = normalize(posFocusID1F - fvertex);
    vec3 LF2 = normalize(posFocusID2F - fvertex);
    vec3 LF3 = normalize(posFocusID3F - fvertex);
    
    vec3 fcolor = Ambient() + Difus(N,LE,colorFocusE) + Especular(N,LE,fvertex,colorFocusE) +
                              Difus(N,LF0,colorFocusID0) + Especular(N,LF0,fvertex,colorFocusID0) +
                              Difus(N,LF1,colorFocusID1) + Especular(N,LF1,fvertex,colorFocusID1) +
                              Difus(N,LF2,colorFocusID2) + Especular(N,LF2,fvertex,colorFocusID2) +
                              Difus(N,LF3,colorFocusID3) + Especular(N,LF3,fvertex,colorFocusID3);
    FragColor = vec4(fcolor, 1);
}
