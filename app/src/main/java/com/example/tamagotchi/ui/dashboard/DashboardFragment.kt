package com.example.tamagotchi.ui.dashboard

import android.graphics.drawable.GradientDrawable
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.gestures.Orientation
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.offset
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width

import androidx.compose.material.ExperimentalMaterialApi
import androidx.compose.material.FractionalThreshold
import androidx.compose.material.Text
import androidx.compose.material.rememberSwipeableState
import androidx.compose.material.swipeable
import androidx.compose.material3.Button
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.ComposeView
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.platform.LocalDensity
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.IntOffset
import androidx.compose.ui.unit.dp
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.example.tamagotchi.R
import com.example.tamagotchi.databinding.FragmentDashboardBinding
import kotlin.math.roundToInt

class DashboardFragment : Fragment() {

    private var _binding: FragmentDashboardBinding? = null

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        val dashboardViewModel =
            ViewModelProvider(this).get(DashboardViewModel::class.java)

        _binding = FragmentDashboardBinding.inflate(inflater, container, false)
        val root: View = binding.root

        val textView: TextView = binding.textDashboard
        dashboardViewModel.text.observe(viewLifecycleOwner) {
            textView.text = it
        }
        val rootView = inflater.inflate(R.layout.fragment_notifications, container, false)

        val composeView = rootView.findViewById<ComposeView>(R.id.composeView)

        composeView.setContent {

            Food()
        }
        return rootView
    }

    @Preview
    @OptIn(ExperimentalMaterialApi::class)
    @Composable
    fun Food(){
        val configuration = LocalConfiguration.current
        val screenHeight = configuration.screenHeightDp.dp
        val screenWidth = configuration.screenWidthDp.dp
        val swipeableStateX = rememberSwipeableState(0)
        val swipeableStateY = rememberSwipeableState(0)
        val anchorsX = mapOf(0f to 0,  with(LocalDensity.current){screenWidth.toPx() to 1})
        val anchorsY = mapOf(0f to 0, with(LocalDensity.current){-screenHeight.toPx() to 1})
        val density = LocalDensity.current

        Box(modifier = Modifier
            .height(screenHeight)
            .width(screenWidth)
            .background(color = Color.White)
            .swipeable(
                state = swipeableStateX,
                anchors = anchorsX,
                thresholds = { _, _ -> FractionalThreshold(0.5f) },
                orientation = Orientation.Horizontal
            )
            .swipeable(
                state = swipeableStateY,
                anchors = anchorsY,
                thresholds = { _, _ -> FractionalThreshold(0.5f) },
                orientation = Orientation.Vertical
            )

        ){
            Text("Crack ")
                Image(

                    modifier = Modifier
                        .size(200.dp)
                        .offset {
                            IntOffset(
                                swipeableStateX.offset.value.roundToInt(),
                                swipeableStateY.offset.value.roundToInt()
                            )
                        }
                    ,
                    painter = painterResource(R.drawable.radish),
                    contentDescription = "Contact profile picture",
                )
        }


        val tolerance = 1.0f // Set an appropriate tolerance value

        if (swipeableStateY.offset.value < with(density) { (-screenHeight.toPx()) } + tolerance) {
            Log.d("debug", "help me")
            LaunchedEffect(Unit) {
                swipeableStateY.snapTo(0)
            }
        }
        Log.d("debug", "swipeableStateY offset: ${swipeableStateY.offset.value}")


    }



    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}