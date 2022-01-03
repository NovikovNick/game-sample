#ifndef GAME_OPENGL_GRAPHIC_PROVIDER_H
#define GAME_OPENGL_GRAPHIC_PROVIDER_H

#include <functional>

#include <glm/glm.hpp>

#include <model/GameObject.h>
#include <model/Input.h>
#include <model/FrameData.h>

class GraphicProvider {
public:
    virtual void init() = 0;

    virtual GameObject *register_objects() const = 0;

    virtual void destroy_objects(GameObject *gameObject) const = 0;

    virtual void render(GameObject *gameObject, glm::mat4 MVP) const = 0;

    virtual void start(std::function<void(const GraphicProvider *provider,
                                          const Input input,
                                          const FrameData frame,
                                          const glm::mat4 View)> f) const = 0;
};

#endif //GAME_OPENGL_GRAPHIC_PROVIDER_H
