package com.bar.vkview;

import com.bar.vkview.ComponentSizeChooser;

public class SimpleEGLConfigChooser extends ComponentSizeChooser {
    public SimpleEGLConfigChooser(boolean withDepthBuffer) {
        super(8, 8, 8, 0, withDepthBuffer ? 16 : 0, 0);
    }
}

