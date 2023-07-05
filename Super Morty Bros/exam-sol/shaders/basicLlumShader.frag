#version 330 core

in vec4 vertexSCO;
in vec3 normalSCO;
in vec3 fmatamb;
in vec3 fmatdiff;
in vec3 fmatspec;
in float fmatshin;

out vec4 FragColor;

// Valors per als components que necessitem dels focus de llum
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);


uniform vec3 posFocusCam;
uniform vec3 posFocusEsc1;
uniform vec3 posFocusEsc2;

uniform vec3 colFocusCam;
uniform vec3 colFocusEsc1;
uniform vec3 colFocusEsc2;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

uniform vec3 colFocus;

vec3 Ambient() {
    return llumAmbient * fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus) 
{
    // Fixeu-vos que SOLS es retorna el terme difús
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
    vec3 colRes = vec3(0);
    // Càlcul component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colFocus * fmatdiff * dot (L, NormSCO);
    return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec4 vertSCO, vec3 colFocus) 
{
    // Fixeu-vos que SOLS es retorna el terme especular!
    // Assumim que els vectors estan normalitzats
    vec3 colRes = vec3 (0);
    // Si la llum ve de darrera o el material és mate no fem res
    if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
      return colRes;  // no hi ha component especular

    // Calculem R i V
    vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
    vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
      return colRes;  // no hi ha component especular
    
    float shine = pow(max(0.0, dot(R, V)), fmatshin);
    return (colRes + fmatspec * colFocus * shine);
}

void main()
{	

    vec3 NM = normalize(normalSCO);

    vec3 LNC = posFocusCam - vertexSCO.xyz;
    LNC = normalize(LNC);

    vec3 pFE1 = (view * vec4(posFocusEsc1,1.0)).xyz;
    vec3 LNE1 = pFE1 - vertexSCO.xyz;
    LNE1 = normalize(LNE1);

    vec3 pFE2 = (view * vec4(posFocusEsc2,1.0)).xyz;
    vec3 LNE2 = pFE2 - vertexSCO.xyz;
    LNE2 = normalize(LNE2);


  vec3 color = Ambient() + Difus(NM,LNC,colFocusCam) + Especular(NM,LNC,vertexSCO,colFocusCam) + Especular(NM,LNE1,vertexSCO,colFocusEsc1) + Difus(NM,LNE2,colFocusEsc2);
  //------------------
    FragColor = vec4(color,1);
    //FragColor = vec4(fmatdiff,1);
}
