#ifndef GAME_OPENGL_GRAPHIC_PROVIDER_H
#define GAME_OPENGL_GRAPHIC_PROVIDER_H

#include <functional>

#include <glm/glm.hpp>

#include <model/GraphicMetaData.h>
#include <model/Input.h>
#include <model/FrameData.h>

class GraphicProvider {
public:
    virtual void Init() = 0;

    virtual GraphicMetaData *GetCubeGraphicMetaData() = 0;

    virtual void Render(GraphicMetaData *gameObject, glm::mat4 MVP) const = 0;

    virtual void OnMainLoop(std::function<void(const GraphicProvider *provider,
                                               const Input input,
                                               const FrameData frame,
                                               const glm::mat4 View)> f) = 0;
};

#endif //GAME_OPENGL_GRAPHIC_PROVIDER_H
