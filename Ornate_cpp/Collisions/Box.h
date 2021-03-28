#pragma once
#include <Math/Model_matrix.h>

#include "Ball.h"
#include "Rectangular_plane.h"

namespace col
{
	struct Box
	{
		bool operator ==(const Box& to_compare)const
		{
			return coordines_as_matrix == to_compare.coordines_as_matrix;
		}
		float form_center_to_corner()const
		{
			return (coordines_as_matrix * mth::vec3(1, 1, 1)).length();
		}
		std::array<mth::vec3, 8> get_vertices()const
		{
			std::array<mth::vec3, 8> result;
			result[0] = mth::vec3(1,1,1);

			result[1] = mth::vec3(1, 1, -1);
			result[2] = mth::vec3(1, -1, 1);
			result[3] = mth::vec3(-1, 1, 1);

			result[4] = mth::vec3(1, -1, -1);
			result[5] = mth::vec3(-1, -1, 1);
			result[6] = mth::vec3(-1, 1, -1);

			result[7] = mth::vec3(-1, -1, -1);

			for (auto& v : result)
				v = coordines_as_matrix * v;
			return result;
		}
		std::array<Rectangular_plane, 6> get_walls()const
		{
			using namespace mth::geo3;
			std::array<mth::vec3, 6> directions{ rigth,up,front,left,bottom,back };
			for (auto& v : directions)
				v = coordines_as_matrix.multiply_direction(v);
			mth::vec3 center = coordines_as_matrix * mth::vec3(0, 0, 0);

			std::array<Rectangular_plane, 6> result;
			result[0] = { Oriented_plane{Front_up{directions[0],directions[1]},directions[0]+ center},mth::vec2(directions[2].length(),directions[1].length()) };
			result[1] = { Oriented_plane{Front_up{directions[1],directions[2]},directions[1]+ center},mth::vec2(directions[0].length(),directions[2].length()) };
			result[2] = { Oriented_plane{Front_up{directions[2],directions[0]},directions[2]+ center},mth::vec2(directions[1].length(),directions[0].length()) };

			result[3] = { Oriented_plane{Front_up{directions[3],directions[4]},directions[3]+ center},mth::vec2(directions[5].length(),directions[4].length()) };
			result[4] = { Oriented_plane{Front_up{directions[4],directions[5]},directions[4]+ center},mth::vec2(directions[3].length(),directions[5].length()) };
			result[5] = { Oriented_plane{Front_up{directions[5],directions[3]},directions[5]+ center},mth::vec2(directions[4].length(),directions[3].length()) };

			return result;
		}
		std::array<mth::geo3::Direction, 6> get_directions()const
		{
			using namespace mth::geo3;
			std::array<Direction, 6> result{ rigth,up,front,left,bottom,back};

			for (auto& v : result)
				v = coordines_as_matrix.multiply_direction(v.vec());
			return result;
		}
		struct Corner
		{
			size_t vertex;
			std::array<size_t, 3> directions;
		};
		constexpr std::array<Corner, 8> get_corners()const
		{
			std::array<Corner, 8> result;
			result[0] = { 0,{{0,1,2}} };
			result[1] = { 1,{{0,5,1}} };
			result[2] = { 3,{{1,2,3}} };
			result[3] = { 6,{{1,3,5}} };

			result[4] = { 2,{{0,2,4}} };
			result[5] = { 4,{{0,4,5}} };
			result[6] = { 5,{{2,3,4}} };
			result[7] = { 7,{{3,4,5}} };
			return result;
		}
		
		constexpr std::array<std::pair<size_t,size_t>, 12> get_edges()const
		{
			std::array<std::pair<size_t, size_t>, 12> result;
			result[0] = { 0,1 };
			result[1] = { 0,2 };
			result[2] = { 0,3 };

			result[3] = { 1,4 };
			result[4] = { 1,6 };
			result[5] = { 2,4 };
			result[6] = { 2,5 };
			result[7] = { 3,5 };
			result[8] = { 3,6 };

			result[9] = { 4,7 };
			result[10] = { 5,7 };
			result[11] = { 6,7 };

			return result;
		}
		mth::vec3 center()const
		{
			return coordines_as_matrix * mth::vec3(0, 0, 0);
		}

		mth::Model_matrix<float, 3> coordines_as_matrix;
	};

	inline bool is_ball_beyond_box_planes(const Box& box, const Ball& ball)
	{
		auto walls = box.get_walls();
		for (const auto& wall : walls)
			if (wall.plane.distance(ball.center) > ball.radius)
				return true;
		return false;
	}

	inline bool are_colliding(const Box& box, const Ball& ball)
	{
		using namespace mth;

		if (is_ball_beyond_box_planes(box, ball))
			return false;

		auto vertices = box.get_vertices();
		auto directions = box.get_directions();
		auto corners = box.get_corners();

		float corner_normalization = mth::vec3(1, 1, 1).length();

		float cone_deegree_as_dot = -geo3::Direction(vec3(1, 1, 1)).dot( vec3(1, 0, 0));
		float counter_dot = trigonometric_identity(cone_deegree_as_dot);

		for (const auto& corner : corners)
		{
			geo3::Direction general_corner_normal{ geo3::Direction::Normalized_mark{} ,
				(directions[corner.directions[0]].vec() +
				directions[corner.directions[1]].vec() +
				directions[corner.directions[2]].vec()) / corner_normalization };
			geo3::Axis corner_axis(general_corner_normal, vertices[corner.vertex]);

			geo3::Direction corner_to_ball_center = ball.center - vertices[corner.vertex];
			if (corner_axis.direction.dot(corner_to_ball_center) < cone_deegree_as_dot)
				continue;

			geo3::Direction corner_axis_to_ball_direction = corner_axis.translation(ball.center);

			geo3::Direction closest_axis_on_cone = general_corner_normal * cone_deegree_as_dot + corner_axis_to_ball_direction*counter_dot;

			if (geo3::Axis(closest_axis_on_cone, vertices[corner.vertex]).distance(ball.center) > ball.radius)
				return false;

			vec3 corner_to_ball = ball.center - corner_axis.center;
			if (corner_to_ball.dot(geo3::Direction(corner_axis.direction)) > 0 && corner_to_ball.length() > ball.radius)
				return false;
		}
		return true;
	}

	inline bool are_colliding(const Box& box1, const Box& box2)
	{
		using namespace mth::geo3;

		mth::Model_matrix<float, 3> to_box1_unit_space = box2.coordines_as_matrix.inversed() * box1.coordines_as_matrix.inversed();
		Ball unit_ball{ mth::vec3(0,0,0),mth::vec3(1, 1, 1).length() };
		if (is_ball_beyond_box_planes({ to_box1_unit_space }, unit_ball))
			return false;

		auto box1_walls = box1.get_walls();
		auto box1_vertices = box1.get_vertices();
		auto box1_edges = box1.get_edges();

		auto box2_walls = box2.get_walls();
		auto box2_vertices = box2.get_vertices();
		auto box2_edges = box2.get_edges();
		
		for (const auto& edge : box2_edges)
			for (const auto& wall : box1_walls)
				if (wall.is_segment_crossing(box2_vertices[edge.first], box2_vertices[edge.second]))
					return true;

		for (const auto& edge : box1_edges)
			for (const auto& wall : box2_walls)
				if (wall.is_segment_crossing(box1_vertices[edge.first], box1_vertices[edge.second]))
					return true;

		auto is_vertex_inside_walls = [&](const std::array<Rectangular_plane, 6>& walls,const mth::vec3& vertex)
		{
			for (const auto& wall : walls)
				if (wall.plane.distance(vertex) > 0)
					return false;
			return true;
		};

		if (is_vertex_inside_walls(box2_walls, box1.center()))
			return true;
		if (is_vertex_inside_walls(box1_walls, box2.center()))
			return true;
		return false;
	}
}