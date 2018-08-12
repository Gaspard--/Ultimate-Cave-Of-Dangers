#version 130

in vec2 pos;
in vec2 texPos;

out vec2 fragTexPos;

void main()
{
  fragTexPos = texPos;
  gl_Position = vec4(pos, 0.0f, 1.0f);
}
