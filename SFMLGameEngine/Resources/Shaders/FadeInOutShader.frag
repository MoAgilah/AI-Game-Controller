uniform sampler2D texture;
uniform float time;

void main()
{
	vec4 pixel_color = texture2D(texture, gl_TexCoord[0].xy);
	vec4 color = (gl_Color * pixel_color);

	gl_FragColor = vec4( color.r, color.g, color.b, color.a * time);
}