R"( #version 460 core



out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D finalTexture;

uniform float deltaTime;

uniform vec2 iResolution;


vec2 curve(vec2 uv)
{
	//uv = (uv - 0.5) * 2.0;
	//uv *= 1.1;	
	//uv.x *= 1.0 + pow((abs(uv.y) / 5.0), 3.0); // OVER HERE
	//uv.y *= 1.0 + pow((abs(uv.x) / 4.0), 3.0);
	//uv  = (uv / 2.0) + 0.5;
	//uv =  uv *0.92 + 0.04;
	return uv;
}
void main()
{
    vec2 q = gl_FragCoord.xy / iResolution.xy;
    vec2 uv = q;
    uv = curve( uv );
    vec3 oricol = texture( finalTexture, vec2(q.x,q.y) ).xyz;
    vec3 col;
	float x =  sin(0.3*deltaTime+uv.y*21.0)*sin(0.3*deltaTime+uv.y*29.0)*sin(0.3+0.1*deltaTime+uv.y*31.0)*0.0007;

    col.r = texture(finalTexture,vec2(x+uv.x+0.00,uv.y+0.00)).x+0.05;
    col.g = texture(finalTexture,vec2(x+uv.x+0.000,uv.y-0.00)).y+0.05;
    col.b = texture(finalTexture,vec2(x+uv.x-0.00,uv.y+0.000)).z+0.05;
   

    col = clamp(col*0.6+0.4*col*col*1.0,0.0,1.0);

    float vig = (0.0 + 1.0*16.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y));
	col *= vec3(pow(vig,0.20));

    col *= vec3(0.95,1.05,0.95);
	col *= 2.8;

	float scans = clamp( 0.35+0.35*sin(20.0 *deltaTime+uv.y*iResolution.y*1.5), 0.0, 1.0);
	
	float s = pow(scans,1.7);
	col = col*vec3( 0.6+0.4*s) ;

   // col *= 1.0+0.01*sin(110.0*deltaTime); //Blinking Effect
	if (uv.x < 0.0 || uv.x > 1.0)
		col *= 0.0;
	if (uv.y < 0.0 || uv.y > 1.0)
		col *= 0.0;
	
	col*=1.0-0.65*vec3(clamp((mod(gl_FragCoord.x, 2.0)-1.0)*2.0,0.0,1.0));
	
    fragColor = vec4(col,1.0);
}


)"