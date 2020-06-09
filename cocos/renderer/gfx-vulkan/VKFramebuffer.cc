#include "VKStd.h"

#include "VKCommands.h"
#include "VKDevice.h"
#include "VKFramebuffer.h"
#include "VKRenderPass.h"
#include "VKTexture.h"

NS_CC_BEGIN

CCVKFramebuffer::CCVKFramebuffer(GFXDevice *device)
: GFXFramebuffer(device) {
}

CCVKFramebuffer::~CCVKFramebuffer() {
}

bool CCVKFramebuffer::initialize(const GFXFramebufferInfo &info) {
    _renderPass = info.renderPass;
    _colorTextures = info.colorTextures;
    _depthStencilTexture = info.depthStencilTexture;
    _isOffscreen = info.isOffscreen;

    _gpuFBO = CC_NEW(CCVKGPUFramebuffer);
    _gpuFBO->gpuRenderPass = ((CCVKRenderPass *)_renderPass)->gpuRenderPass();

    if (_isOffscreen) {
        _gpuFBO->gpuColorViews.resize(_colorTextures.size());
        for (size_t i = 0; i < _colorTextures.size(); ++i) {
            CCVKTexture *colorview = (CCVKTexture *)_colorTextures[i];
            _gpuFBO->gpuColorViews[i] = colorview->gpuTextureView();
        }

        if (_depthStencilTexture) {
            _gpuFBO->gpuDepthStencilView = ((CCVKTexture *)_depthStencilTexture)->gpuTextureView();
        }

        _gpuFBO->isOffscreen = _isOffscreen;
    }

    CCVKCmdFuncCreateFramebuffer((CCVKDevice *)_device, _gpuFBO);

    _status = GFXStatus::SUCCESS;

    return true;
}

void CCVKFramebuffer::destroy() {
    if (_gpuFBO) {
        if (_isOffscreen) {
            CCVKCmdFuncDestroyFramebuffer((CCVKDevice *)_device, _gpuFBO);
        }
        CC_DELETE(_gpuFBO);
        _gpuFBO = nullptr;
    }

    _status = GFXStatus::UNREADY;
}

NS_CC_END
