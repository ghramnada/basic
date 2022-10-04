#include "pch.h"
#include "property_panel.h"

namespace nui
{
  void Property_Panel::render(nui::SceneView* scene_view)
  {
    auto mesh = scene_view->get_mesh();

    ImGui::Begin("Properties");
    if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    {
      
      if (ImGui::Button("Open..."))
      {
        mFileDialog.Open();
      }
      ImGui::SameLine(0, 5.0f);
      ImGui::Text(mCurrentFile.c_str());
    }

    if (ImGui::CollapsingHeader("Material") && mesh)
    {
      ImGui::ColorPicker3("Color", (float*)&mesh->mColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
      ImGui::SliderFloat("Roughness", &mesh->mRoughness, 0.0f, 1.0f);
      ImGui::SliderFloat("Metallic", &mesh->mMetallic, 0.0f, 1.0f);
    }

    if (ImGui::CollapsingHeader("Light"))
    {

      ImGui::Separator();
      ImGui::Text("Position");
      ImGui::Separator();
      nimgui::draw_vec3_widget("Position", scene_view->get_light()->mPosition, 80.0f);
    }

    if (ImGui::CollapsingHeader("reNdering_mode"))
    {
        const char* items[] = { "points","lines","faces"};
        static const char* current_item = NULL;

        if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(items[n], is_selected)) {
                    current_item = items[n];
                }
                ImGui::SetItemDefaultFocus();
                if (current_item == "points") {
                    mesh->mode = GL_POINTS;
                }
                else if (current_item == "lines") {
                    mesh->mode = GL_LINES;
                }
                else if(current_item == "faces") {
                    mesh->mode = GL_TRIANGLES;
                }
                
            }
            ImGui::EndCombo();
        }
        
    }

    ImGui::End();

    mFileDialog.Display();
    if (mFileDialog.HasSelected())
    {
      auto file_path = mFileDialog.GetSelected().string();
      mCurrentFile = file_path.substr(file_path.find_last_of("/\\") + 1);

      mMeshLoadCallback(file_path);

      mFileDialog.ClearSelected();
    }

  }
}
