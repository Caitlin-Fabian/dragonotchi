package com.example.tamagotchi.ui.home


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
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.material.ExperimentalMaterialApi
import androidx.compose.material.FractionalThreshold
import androidx.compose.material.Text
import androidx.compose.material.rememberSwipeableState
import androidx.compose.material.swipeable
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect

import androidx.compose.runtime.snapshots.Snapshot

import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.ComposeView
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.platform.LocalDensity
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.font.Font
import androidx.compose.ui.text.font.FontFamily
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.IntOffset
import androidx.compose.ui.unit.dp
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.example.tamagotchi.R
import com.example.tamagotchi.databinding.FragmentHomeBinding
import com.google.android.gms.tasks.OnSuccessListener
import com.google.android.gms.tasks.Task
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.ktx.database
import com.google.firebase.ktx.Firebase
import kotlin.math.roundToInt

class HomeFragment : Fragment() {

    private var _binding: FragmentHomeBinding? = null

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        val homeViewModel =
            ViewModelProvider(this).get(HomeViewModel::class.java)

        _binding = FragmentHomeBinding.inflate(inflater, container, false)
        val root: View = binding.root

        val textView: TextView = binding.textHome
        homeViewModel.text.observe(viewLifecycleOwner) {
            textView.text = it
        }

        val rootView = inflater.inflate(R.layout.fragment_notifications, container, false)

        val composeView = rootView.findViewById<ComposeView>(R.id.composeView)

        composeView.setContent {

            Play()
        }
        return rootView

    }

    @OptIn(ExperimentalMaterialApi::class)
    @Preview
    @Composable
    fun Play(){

        val configuration = LocalConfiguration.current
        val screenHeight = configuration.screenHeightDp.dp
        val screenWidth = configuration.screenWidthDp.dp
        val swipeableStateX = rememberSwipeableState(0)
        val swipeableStateY = rememberSwipeableState(0)
        val anchorsX = mapOf(0f to 0,  with(LocalDensity.current){screenWidth.toPx() to 1})
        val anchorsY = mapOf(0f to 0, with(LocalDensity.current){-screenHeight.toPx() to 1})
        val density = LocalDensity.current
        val youngSerif = FontFamily(
            Font(R.font.young_serif, FontWeight.Bold)
        )
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
            ,
            contentAlignment = Alignment.Center // Center the content

        ){
            Column(verticalArrangement = Arrangement.Center,
                horizontalAlignment = Alignment.CenterHorizontally) {
                Image(
                    modifier = Modifier
                        .size(200.dp)
                        .offset {

                            IntOffset(
                                swipeableStateX.offset.value.roundToInt(),
                                swipeableStateY.offset.value.roundToInt()
                            )
                        },
                    painter = painterResource(R.drawable.red_ball),
                    contentDescription = "Contact profile picture",
                )
                Text( text = "Crack ", fontFamily = youngSerif, fontWeight = FontWeight.Normal,textAlign = TextAlign.Center)
            }
        }


        val tolerance = 1.0f // Set an appropriate tolerance value

        if (swipeableStateY.offset.value < with(density) { (-screenHeight.toPx()) } + tolerance) {

            // ball was swiped
            Log.d("debug", "help me")
            val database = Firebase.database
            val boredomRef = database.getReference("boredomLevel")
            val ballRef = database.getReference("threwBall")

            // attempt to get and update boredom level
            boredomRef.get().addOnSuccessListener {
                Log.i("firebase", "Got value ${it.value}")
                var myInt = it.value.toString().toInt()
                if (myInt in 0..4) {
                    myInt++
                    ballRef.setValue(true)
                    boredomRef.setValue(myInt)
                }
            }.addOnFailureListener{
                Log.e("firebase", "Error getting data", it)
            }

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