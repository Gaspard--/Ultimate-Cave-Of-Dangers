#version 130

in vec2 fragTexPos;
out vec4 color;

void main()
{
  if (dot(fragTexPos, fragTexPos) > 0.5)
    discard;
  color = vec4(0.0f, 1.0f, 1.0f, 0.5f);
}
