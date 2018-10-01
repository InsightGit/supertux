//  SuperTux
//  Copyright (C) 2006 Matthias Braun <matze@braunis.de>
//                2018 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_SUPERTUX_VIDEO_CANVAS_HPP
#define HEADER_SUPERTUX_VIDEO_CANVAS_HPP

#include <string>
#include <vector>
#include <obstack.h>

#include "math/rectf.hpp"
#include "math/vector.hpp"
#include "video/gl.hpp"
#include "video/color.hpp"
#include "video/font.hpp"
#include "video/font_ptr.hpp"
#include "video/drawing_target.hpp"

class DrawingContext;
class Renderer;
class VideoSystem;
struct DrawingRequest;

// some constants for predefined layer values
enum {
  // Image/gradient backgrounds (should cover entire screen)
  LAYER_BACKGROUND0 = -300,
  // Particle backgrounds
  LAYER_BACKGROUND1 = -200,
  // Tilemap backgrounds
  LAYER_BACKGROUNDTILES = -100,
  // Solid tilemaps
  LAYER_TILES = 0,
  // Ordinary objects
  LAYER_OBJECTS = 50,
  // Objects that pass through walls
  LAYER_FLOATINGOBJECTS = 150,
  //
  LAYER_FOREGROUNDTILES = 200,
  //
  LAYER_FOREGROUND0 = 300,
  //
  LAYER_FOREGROUND1 = 400,

  LAYER_LIGHTMAP = 450,

  // Hitpoints, time, coins, etc.
  LAYER_HUD = 500,
  // Menus, mouse, console etc.
  LAYER_GUI         = 600
};

enum GradientDirection { VERTICAL, HORIZONTAL, VERTICAL_SECTOR, HORIZONTAL_SECTOR };

class Blend
{
public:
  GLenum sfactor;
  GLenum dfactor;

  Blend()
    : sfactor(GL_SRC_ALPHA), dfactor(GL_ONE_MINUS_SRC_ALPHA)
  {}

  Blend(GLenum s, GLenum d)
    : sfactor(s), dfactor(d)
  {}
};

class PaintStyle
{
public:
  PaintStyle() :
    m_color(Color::WHITE),
    m_alpha(1.0f),
    m_blend(),
    m_flip(NO_FLIP)
  {}

  PaintStyle& set_color(const Color& color) {
    m_color = color;
    return *this;
  }

  PaintStyle& set_alpha(const float& alpha) {
    m_alpha = alpha;
    return *this;
  }

  PaintStyle& set_blend(const Blend& blend) {
    m_blend = blend;
    return *this;
  }

  PaintStyle& set_flip(const Flip& flip) {
    m_flip = flip;
    return *this;
  }

  const Color& get_color() const { return m_color; }
  const float& get_alpha() const { return m_alpha; }
  const Blend& get_blend() const { return m_blend; }
  const Flip& get_flip() const { return m_flip; }

private:
  Color m_color;
  float m_alpha;
  Blend m_blend;
  Flip m_flip;
};

class Canvas
{
public:
  enum Filter { BELOW_LIGHTMAP, ABOVE_LIGHTMAP, ALL };

public:
  Canvas(DrawingContext& context, obstack& obst);
  ~Canvas();

  void draw_surface(SurfacePtr surface, const Vector& position, int layer);
  void draw_surface(SurfacePtr surface, const Vector& position, float angle, const Color& color, const Blend& blend,
                    int layer);
  void draw_surface_part(SurfacePtr surface, const Rectf& srcrect, const Rectf& dstrect,
                         int layer, const PaintStyle& style = PaintStyle());
  void draw_surface_scaled(SurfacePtr surface, const Rectf& dstrect,
                           int layer, const PaintStyle& style = PaintStyle());
  void draw_surface_batch(SurfacePtr surface,
                          const std::vector<Rectf>& srcrects,
                          const std::vector<Rectf>& dstrects,
                          const Color& color,
                          int layer);
  void draw_text(FontPtr font, const std::string& text,
                 const Vector& position, FontAlignment alignment, int layer, const Color& color = Color(1.0,1.0,1.0));
  /** Draw text to the center of the screen */
  void draw_center_text(FontPtr font, const std::string& text,
                        const Vector& position, int layer, const Color& color = Color(1.0,1.0,1.0));
  void draw_gradient(const Color& from, const Color& to, int layer, const GradientDirection& direction, const Rectf& region);
  void draw_filled_rect(const Vector& topleft, const Vector& size,
                        const Color& color, int layer);
  void draw_filled_rect(const Rectf& rect, const Color& color, int layer);
  void draw_filled_rect(const Rectf& rect, const Color& color, float radius, int layer);

  void draw_inverse_ellipse(const Vector& pos, const Vector& size, const Color& color, int layer);

  void draw_line(const Vector& pos1, const Vector& pos2, const Color& color, int layer);
  void draw_triangle(const Vector& pos1, const Vector& pos2, const Vector& pos3, const Color& color, int layer);

  /** on next update, set color to lightmap's color at position */
  void get_pixel(const Vector& position, Color* color_out);

  void clear();
  void render(Renderer& renderer, Filter filter);

  DrawingContext& get_context() { return m_context; }

  // FIXME: this function is temporary only, should be refactored out
  std::vector<DrawingRequest*>& get_requests() { return m_requests; }

private:
  Vector apply_translate(const Vector& pos) const;

private:
  DrawingContext& m_context;
  obstack& m_obst;
  std::vector<DrawingRequest*> m_requests;

private:
  Canvas(const Canvas&) = delete;
  Canvas& operator=(const Canvas&) = delete;
};

#endif

/* EOF */
