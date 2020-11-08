//
// Created by nathan on 14/10/20.
//

#include "ModelNode.h"

void ModelNode::transform() {

}

void ModelNode::render(glm::mat4 &viewMat, glm::mat4 &projectionMat, const glm::vec3 &lightPos) {

    material->activate();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    for(auto& attrib : layout){
        glEnableVertexAttribArray(attrib.handle);
        glVertexAttribPointer(attrib.handle, attrib.element_size, attrib.type,
                              attrib.major,
                              attrib.byte_stride, reinterpret_cast<const void *>(attrib.offset));

    }
/*
    glm::mat4 viewT = viewMat*activeTransform.get();
    glm::mat4 projT = projectionMat*viewT;


    //draw triangles
    glUniformMatrix4fv(mMVPMatrixHandle, 1, false, glm::value_ptr(projT));
    glUniformMatrix4fv(mMVMatrixHandle, 1, false, glm::value_ptr(viewT));
    //glUniformMatrix4fv(mNormalMatrixHandle, 1, false, glm::value_ptr(viewMat));

    glUniform3fv(mLightPosHandle, 1, glm::value_ptr(lightPos));
    glUniform1f(sunSurface->getUniform("u_diffuseCoaff"), 0.1);
    glUniform1f(sunSurface->getUniform("u_specularCoaff"), 0.9);
    glUniform1f(sunSurface->getUniform("u_shininess"), 5.0);
    glUniform1f(sunSurface->getUniform("u_ambiantCoaff"), 0.2);


    glUniform3fv(mBrickColorHandle, 1, brickColor[m_idx]);
    glUniform3fv(mMortarColorHandle, 1, mortarColor[m_idx]);
    glUniform2fv(mBrickSizeHandle, 1, brickSize[m_idx]);
    glUniform2fv( mBrickPctHandle, 1, brickPct[m_idx]);


    for(auto indexBuffer:indexGroups){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.first);
        std::tuple<unsigned, unsigned, float> params =indexBuffer.second;
        glDrawElements(params.get(0), params.b,indexBuffer.second.[0]);
    }


    glDisableVertexAttribArray(positionAttributeHandle);
    //glDisableVertexAttribArray(colorAttributeHandle);
    //glDisableVertexAttribArray(normalAttributeHandle);
    //glBindTexture(GL_TEXTURE_3D, 0);
    for(auto& attrib : layout) {
        glDisableVertexAttribArray(attrib.handle);
    }
    */
    material->deactivate();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexGroups.size());

}
