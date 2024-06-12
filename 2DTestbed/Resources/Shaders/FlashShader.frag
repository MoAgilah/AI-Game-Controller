uniform sampler2D texture;

uniform float time;
uniform vec4 flashColor;

void main()
{
	vec4 pixel_color = texture2D(texture, gl_TexCoord[0].xy);

	vec4 lerp_color = mix(pixel_color, flashColor,  0 + 0.5 *sin(time*50.0));

	gl_FragColor = lerp_color * pixel_color.a;
}